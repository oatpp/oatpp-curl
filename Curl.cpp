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

#include "Curl.hpp"

namespace oatpp { namespace curl {
  
CurlHeaders::CurlHeaders()
  : m_list(nullptr)
{}

CurlHeaders::~CurlHeaders() {
  if(m_list != nullptr) {
    curl_slist_free_all(m_list);
  }
}
  
void CurlHeaders::append(const oatpp::String& key, const oatpp::String& value) {
  oatpp::String headerEntry = key + ": " + value;
  m_list = curl_slist_append(m_list, headerEntry->c_str());
}
  
}}
