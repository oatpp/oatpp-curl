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

#ifndef oatpp_curl_CurlHeadersReader_hpp
#define oatpp_curl_CurlHeadersReader_hpp

#include "./Curl.hpp"

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/data/stream/ChunkedBuffer.hpp"

namespace oatpp { namespace curl { namespace io {
  
class CurlHeadersReader {
public:
  constexpr static v_int32 STATE_INITIALIZED = 0;
  constexpr static v_int32 STATE_STARTED = 1;
  constexpr static v_int32 STATE_FINISHED = 2;
private:
  std::shared_ptr<CurlHandles> m_handles;
  os::io::Library::v_size m_position;
  v_int32 m_state;
  oatpp::web::protocol::http::Protocol::Headers m_headers;
  oatpp::web::protocol::http::ResponseStartingLine m_startingLine;
  oatpp::data::stream::ChunkedBuffer m_buffer;
private:
  static size_t headerCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
public:
  
  CurlHeadersReader(const std::shared_ptr<CurlHandles>& curlHandles)
    : m_handles(curlHandles)
    , m_position(0)
    , m_state(STATE_INITIALIZED)
  {
    curl_easy_setopt(m_handles->getEasyHandle(), CURLOPT_HEADERFUNCTION, headerCallback);
    curl_easy_setopt(m_handles->getEasyHandle(), CURLOPT_HEADERDATA, this);
  }
  
  v_int32 getState() {
    return m_state;
  }
  
  const oatpp::web::protocol::http::ResponseStartingLine& getStartingLine() const {
    return m_startingLine;
  }
  
  const oatpp::web::protocol::http::Protocol::Headers& getHeaders() const {
    return m_headers;
  }
  
};
  
}}}

#endif /* oatpp_curl_CurlHeadersReader_hpp */
