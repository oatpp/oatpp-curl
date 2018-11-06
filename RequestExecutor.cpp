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

#include "CurlReader.hpp"
#include "CurlWriter.hpp"

#include <curl/curl.h>

namespace oatpp { namespace curl {
  
  std::shared_ptr<RequestExecutor::Response> RequestExecutor::execute(const String& method,
                                                                      const String& path,
                                                                      const std::shared_ptr<Headers>& userDefinedHeaders,
                                                                      const std::shared_ptr<Body>& body)
  {
  
    oatpp::String host = "http://httpbin.org/put";
    oatpp::String url = host;// + path;
    
    auto curl = std::make_shared<CurlHandles>();
    CurlReader reader(curl);
    CurlWriter writer(curl);
    
    CURLcode res;
      
    struct curl_slist *curlHeaders = nullptr;
    
    /*
    if(userDefinedHeaders) {
      auto currHeader = userDefinedHeaders->getFirstEntry();
      while (currHeader != nullptr) {
        oatpp::String headerEntry = currHeader->getKey() + ": " + currHeader->getValue();
        curlHeaders = curl_slist_append(curlHeaders, headerEntry->c_str());
        currHeader = currHeader->getNext();
      }
    }
    */
    
    curlHeaders = curl_slist_append(curlHeaders, "Expect:");
    //curlHeaders = curl_slist_append(curlHeaders, "Content-Type: text/plain");
    
    curl_easy_setopt(curl->getEasyHandle(), CURLOPT_URL, url->c_str());
    curl_easy_setopt(curl->getEasyHandle(), CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl->getEasyHandle(), CURLOPT_HTTPHEADER, curlHeaders);
    curl_easy_setopt(curl->getEasyHandle(), CURLOPT_UPLOAD, 1L);
    
    writer.write("long long data goes here", 24);
    
    auto chunkedBuffer = oatpp::data::stream::ChunkedBuffer::createShared();
    v_char8 buffer [256];
    v_int64 count = 0;
    while ((count = reader.read(buffer, 256)) > 0) {
      chunkedBuffer->write(buffer, count);
      OATPP_LOGD("curl", "iteration count=%d", count);
    }
    
    OATPP_LOGD("result", "data='%s'", chunkedBuffer->toString()->c_str());
  
    if(curlHeaders != nullptr) {
      curl_slist_free_all(curlHeaders);
    }
    
    
    return nullptr;
  }
  
}}
