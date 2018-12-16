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

#include "BodyDecoder.hpp"

namespace oatpp { namespace curl { namespace io {
  
void BodyDecoder::decode(const oatpp::web::protocol::http::Protocol::Headers& headers,
                         const std::shared_ptr<oatpp::data::stream::InputStream>& bodyStream,
                         const std::shared_ptr<oatpp::data::stream::OutputStream>& toStream) const
{
  oatpp::data::buffer::IOBuffer buffer;
  oatpp::data::stream::transfer(bodyStream, toStream, 0, buffer.getData(), buffer.getSize());
}

oatpp::async::Action BodyDecoder::decodeAsync(oatpp::async::AbstractCoroutine* parentCoroutine,
                                              const oatpp::async::Action& actionOnReturn,
                                              const oatpp::web::protocol::http::Protocol::Headers& headers,
                                              const std::shared_ptr<oatpp::data::stream::InputStream>& bodyStream,
                                              const std::shared_ptr<oatpp::data::stream::OutputStream>& toStream) const
{
  auto buffer = oatpp::data::buffer::IOBuffer::createShared();
  return oatpp::data::stream::transferAsync(parentCoroutine, actionOnReturn, bodyStream, toStream, 0, buffer);
}
  
}}}
