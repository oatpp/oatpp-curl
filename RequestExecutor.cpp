/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi, <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#include "RequestExecutor.hpp"

#include "./io/BodyInputStream.hpp"
#include "./io/BodyOutputStream.hpp"
#include "./io/CurlHeadersReader.hpp"
#include "./io/CurlBodyReader.hpp"
#include "./io/CurlBodyWriter.hpp"

#include <curl/curl.h>
#include <chrono>

namespace oatpp { namespace curl {
  
  std::shared_ptr<RequestExecutor::Response> RequestExecutor::execute(const String& method,
                                                                      const String& path,
                                                                      const std::shared_ptr<Headers>& userDefinedHeaders,
                                                                      const std::shared_ptr<Body>& body)
  {
    
    oatpp::String url = m_baseUrl + path;
    
    auto curl = std::make_shared<io::CurlHandles>();
    auto reader = std::make_shared<io::CurlBodyReader>(curl);
    auto writer = std::make_shared<io::CurlBodyWriter>(curl);
    io::CurlHeadersReader headersReader(curl);
    io::CurlHeaders headers;
    std::shared_ptr<Headers> bodyHeaders;
    
    headers.append("Expect", "");
    
    if(userDefinedHeaders) {
      auto currHeader = userDefinedHeaders->getFirstEntry();
      while (currHeader != nullptr) {
        headers.append(currHeader->getKey(), currHeader->getValue());
        currHeader = currHeader->getNext();
      }
    }
    
    if(body) {
      bodyHeaders = Headers::createShared();
      body->declareHeaders(bodyHeaders);
    }
    
    curl_easy_setopt(curl->getEasyHandle(), CURLOPT_URL, url->c_str());
    curl_easy_setopt(curl->getEasyHandle(), CURLOPT_CUSTOMREQUEST, method->c_str());
    curl_easy_setopt(curl->getEasyHandle(), CURLOPT_HTTPHEADER, headers.getCurlList());
    
    if(bodyHeaders) {
      curl_easy_setopt(curl->getEasyHandle(), CURLOPT_UPLOAD, 1L);
      body->writeToStream(std::make_shared<io::BodyOutputStream>(writer));
    }
    
    int still_running = 1;
    curl_multi_perform(curl->getMultiHandle(), &still_running);
    while (still_running && headersReader.getState() != io::CurlHeadersReader::STATE_FINISHED) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      curl_multi_perform(curl->getMultiHandle(), &still_running);
    }
    
    auto line = headersReader.getStartingLine();
    auto responseHeaders = headersReader.getHeaders();
    
    auto bodyStream = std::make_shared<io::BodyInputStream>(reader, false);
    
    return Response::createShared(line->statusCode, line->description, responseHeaders, bodyStream, m_bodyDecoder);
  }
  
  oatpp::async::Action RequestExecutor::executeAsync(oatpp::async::AbstractCoroutine* parentCoroutine,
                                                     AsyncCallback callback,
                                                     const String& method,
                                                     const String& path,
                                                     const std::shared_ptr<Headers>& headers,
                                                     const std::shared_ptr<Body>& body)
  {
    
    class ExecutorCoroutine : public oatpp::async::CoroutineWithResult<ExecutorCoroutine, std::shared_ptr<Response>> {
    private:
      std::shared_ptr<Body> m_body;
      std::shared_ptr<const oatpp::web::protocol::http::incoming::BodyDecoder> m_bodyDecoder;
    private:
      std::shared_ptr<io::CurlHandles> m_curl;
      std::shared_ptr<io::CurlBodyReader> m_reader;
      std::shared_ptr<io::CurlBodyWriter> m_writer;
      std::shared_ptr<io::CurlHeadersReader> m_headersReader;
      io::CurlHeaders m_curlHeaders;
      
    public:
      
      ExecutorCoroutine(const oatpp::String& url,
                        const String& method,
                        const std::shared_ptr<Headers>& headers,
                        const std::shared_ptr<Body>& body,
                        const std::shared_ptr<const oatpp::web::protocol::http::incoming::BodyDecoder>& bodyDecoder)
        : m_body(body)
        , m_bodyDecoder(bodyDecoder)
      {
        
        m_curl = std::make_shared<io::CurlHandles>();
        m_reader = std::make_shared<io::CurlBodyReader>(m_curl);
        m_writer = std::make_shared<io::CurlBodyWriter>(m_curl);
        m_headersReader = std::make_shared<io::CurlHeadersReader>(m_curl);
        
        std::shared_ptr<Headers> bodyHeaders;
        
        m_curlHeaders.append("Expect", "");
        
        if(headers) {
          auto currHeader = headers->getFirstEntry();
          while (currHeader != nullptr) {
            m_curlHeaders.append(currHeader->getKey(), currHeader->getValue());
            currHeader = currHeader->getNext();
          }
        }
        
        curl_easy_setopt(m_curl->getEasyHandle(), CURLOPT_URL, url->c_str());
        curl_easy_setopt(m_curl->getEasyHandle(), CURLOPT_CUSTOMREQUEST, method->c_str());
        curl_easy_setopt(m_curl->getEasyHandle(), CURLOPT_HTTPHEADER, m_curlHeaders.getCurlList());
        
        if(m_body) {
          bodyHeaders = Headers::createShared();
          m_body->declareHeaders(bodyHeaders);
          curl_easy_setopt(m_curl->getEasyHandle(), CURLOPT_UPLOAD, 1L);
        }
        
      }
      
      Action act() override {
        if(m_body) {
          return m_body->writeToStreamAsync(this, yieldTo(&ExecutorCoroutine::doPerform), std::make_shared<io::BodyOutputStream>(m_writer, true /* non-blocking */));
        }
        return yieldTo(&ExecutorCoroutine::doPerform);
      }
      
      Action doPerform() {
        
        if(m_headersReader->getState() != io::CurlHeadersReader::STATE_FINISHED) {
          int still_running = 1;
          curl_multi_perform(m_curl->getMultiHandle(), &still_running);
          if(still_running){
            return waitRetry();
          }
        }
        
        auto line = m_headersReader->getStartingLine();
        auto responseHeaders = m_headersReader->getHeaders();
        auto bodyStream = std::make_shared<io::BodyInputStream>(m_reader, true /* non-blocking */);
        
        return _return(Response::createShared(line->statusCode, line->description, responseHeaders, bodyStream, m_bodyDecoder));
        
      }
      
    };
    
    return parentCoroutine->startCoroutineForResult<ExecutorCoroutine>(callback, m_baseUrl + path, method, headers, body, m_bodyDecoder);
    
  }
  
}}
