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

#include "BodyInputStream.hpp"
#include "CurlHeadersReader.hpp"
#include "CurlBodyReader.hpp"
#include "CurlBodyWriter.hpp"

#include <curl/curl.h>
#include <chrono>

namespace oatpp { namespace curl {
  
  std::shared_ptr<RequestExecutor::Response> RequestExecutor::execute(const String& method,
                                                                      const String& path,
                                                                      const std::shared_ptr<Headers>& userDefinedHeaders,
                                                                      const std::shared_ptr<Body>& body)
  {
    
    oatpp::String url = m_baseUrl + path;
    
    auto curl = std::make_shared<CurlHandles>();
    auto reader = std::make_shared<CurlBodyReader>(curl);
    CurlBodyWriter writer(curl);
    CurlHeadersReader headersReader(curl);
    CurlHeaders headers;
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
      writer.write("long long data goes here", 24);
    }
    
    int still_running = 1;
    curl_multi_perform(curl->getMultiHandle(), &still_running);
    while (still_running && headersReader.getState() == CurlHeadersReader::STATE_INITIALIZED) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      curl_multi_perform(curl->getMultiHandle(), &still_running);
    }
    
    if(!still_running) {
      throw std::runtime_error("[oatpp::curl::RequestExecutor::execute()]: Unknown error.");
    }
    
    while (still_running && headersReader.getState() != CurlHeadersReader::STATE_FINISHED) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      curl_multi_perform(curl->getMultiHandle(), &still_running);
    }
    
    if(!still_running) {
      throw std::runtime_error("[oatpp::curl::RequestExecutor::execute()]: Unknown error.");
    }
    
    auto line = headersReader.getStartingLine();
    auto responseHeaders = headersReader.getHeaders();
    
    auto bodyStream = std::make_shared<BodyInputStream>(reader, false);
    
    return Response::createShared(line->statusCode, line->description, responseHeaders, bodyStream);
  }
  
}}
