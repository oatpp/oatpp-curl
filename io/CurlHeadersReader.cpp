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

namespace oatpp { namespace curl { namespace io {
  
size_t CurlHeadersReader::headerCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  
  oatpp::parser::ParsingCaret caret((p_char8)ptr, (v_int32)(size * nmemb));
  
  CurlHeadersReader* instance = static_cast<CurlHeadersReader*>(userdata);
  
  if(instance->m_state == STATE_INITIALIZED) {
    instance->m_state = STATE_STARTED;
    instance->m_startingLine = oatpp::web::protocol::http::Protocol::parseResponseStartingLine(caret);
  } else if(instance->m_state == STATE_STARTED) {
    if(caret.isAtRN()) {
      instance->m_state = STATE_FINISHED;
    }
    oatpp::web::protocol::http::Status error;
    oatpp::web::protocol::http::Protocol::parseOneHeader(*instance->m_headers, caret, error);
  } else if(instance->m_state == STATE_FINISHED) {
    throw std::runtime_error("[oatpp::curl::CurlHeadersReader::headerCallback(...)]: Invalid state.");
  }
  
  return caret.getSize();
}
  
}}}
