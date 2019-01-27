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

#ifndef oatpp_curl_Curl_hpp
#define oatpp_curl_Curl_hpp

#include "oatpp/core/Types.hpp"

#include <curl/curl.h>

namespace oatpp { namespace curl { namespace io {
  
class CurlHeaders {
private:
  curl_slist* m_list;
public:
  
  CurlHeaders();
  ~CurlHeaders();
  
  void append(const oatpp::String& key, const oatpp::String& value);
  
  curl_slist* getCurlList() {
    return m_list;
  }
  
};
  
class CurlHandles {
private:
  CURL* m_easyhandle;
  CURLM* m_multiHandle; // curl-multi is used for non-blocking perform
public:
  
  CurlHandles()
    : m_easyhandle(curl_easy_init())
    , m_multiHandle(curl_multi_init())
  {
    curl_multi_add_handle(m_multiHandle, m_easyhandle);
  }
  
  ~CurlHandles() {
    curl_multi_remove_handle(m_multiHandle, m_easyhandle);
    curl_easy_cleanup(m_easyhandle);
    curl_multi_cleanup(m_multiHandle);
  }
  
  CURL* getEasyHandle() {
    return m_easyhandle;
  }
  
  CURLM* getMultiHandle() {
    return m_multiHandle;
  }
  
};
  
}}}

#endif /* oatpp_curl_Curl_hpp */
