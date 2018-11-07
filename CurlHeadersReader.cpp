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

#include "CurlHeadersReader.hpp"

namespace oatpp { namespace curl {
  
size_t CurlHeadersReader::headerCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  //CurlHeadersReader* instance = static_cast<CurlHeadersReader*>(userdata);
  v_int32 strSize = (v_int32)(size * nmemb);
  oatpp::String header(ptr, strSize, true);
  OATPP_LOGD("curl", "headers::callback(). header='%s'", header->c_str());
  return header->getSize();
}
  
}}
