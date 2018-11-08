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

#ifndef oatpp_curl_BodyDecoder_hpp
#define oatpp_curl_BodyDecoder_hpp

#include "oatpp/web/protocol/http/incoming/BodyDecoder.hpp"

namespace oatpp { namespace curl { namespace io {
  
class BodyDecoder : public oatpp::web::protocol::http::incoming::BodyDecoder {
public:
  
  /**
   * Just transfer everything we have in @bodyStream to @toStream as-is
   * Curl already did all decoding
   */
  void decode(const std::shared_ptr<oatpp::web::protocol::http::Protocol::Headers>& headers,
              const std::shared_ptr<oatpp::data::stream::InputStream>& bodyStream,
              const std::shared_ptr<oatpp::data::stream::OutputStream>& toStream) const override;
  
  /**
   * Just transfer everything we have in @bodyStream to @toStream as-is
   * Curl already did all decoding
   */
  oatpp::async::Action decodeAsync(oatpp::async::AbstractCoroutine* parentCoroutine,
                                   const oatpp::async::Action& actionOnReturn,
                                   const std::shared_ptr<oatpp::web::protocol::http::Protocol::Headers>& headers,
                                   const std::shared_ptr<oatpp::data::stream::InputStream>& bodyStream,
                                   const std::shared_ptr<oatpp::data::stream::OutputStream>& toStream) const override;
  
};
  
}}}

#endif /* oatpp_curl_BodyDecoder_hpp */
