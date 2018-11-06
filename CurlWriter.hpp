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

#ifndef oatpp_curl_CurlWriter_hpp
#define oatpp_curl_CurlWriter_hpp

#include "./Curl.hpp"

#include "oatpp/core/data/stream/Stream.hpp"

#include <memory>

namespace oatpp { namespace curl {
  
class CurlWriter {
private:
  std::shared_ptr<CurlHandles> m_handles;
  const void* m_currentData;
  os::io::Library::v_size m_currentDataSize;
private:
  static size_t readCallback(char *buffer, size_t size, size_t nitems, void *userdata);
public:
  
  CurlWriter(const std::shared_ptr<CurlHandles>& curlHandles)
    : m_handles(curlHandles)
    , m_currentData(nullptr)
    , m_currentDataSize(0)
  {
    curl_easy_setopt(m_handles->getEasyHandle(), CURLOPT_READFUNCTION, readCallback);
    curl_easy_setopt(m_handles->getEasyHandle(), CURLOPT_READDATA, this);
  }
  
  os::io::Library::v_size write(const void *data, os::io::Library::v_size count);
  os::io::Library::v_size writeNonBlocking(const void *data, os::io::Library::v_size count);
  
};
  
}}

#endif /* oatpp_curl_CurlWriter_hpp */
