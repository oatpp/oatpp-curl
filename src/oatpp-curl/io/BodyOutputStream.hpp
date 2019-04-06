/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
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

#ifndef oatpp_curl_BodyOutputStream_hpp
#define oatpp_curl_BodyOutputStream_hpp

#include "CurlBodyWriter.hpp"

namespace oatpp { namespace curl { namespace io {

/**
 * Wrapper over &id:oatpp::curl::io::CurlBodyWriter; providing &id:oatpp::data::stream::OutputStream; interface.
 */
class BodyOutputStream : public oatpp::data::stream::OutputStream {
private:
  std::shared_ptr<CurlBodyWriter> m_writer;
  bool m_nonBlocking;
public:

  /**
   * Constructor.
   * @param nonBlocking - `true` for non-blocking writes.
   */
  BodyOutputStream(const std::shared_ptr<CurlBodyWriter>, bool nonBlocking = false);

  /**
   * Write data to stream. Implementation of &id:oatpp::data::stream::OutputStream::write; method.
   * @param data - data to write.
   * @param count - data size.
   * @return - actual amount of bytes written. &id:oatpp::data::v_io_size;.
   */
  data::v_io_size write(const void *data, data::v_io_size count) override;
  
};
  
}}}

#endif /* oatpp_curl_BodyOutputStream_hpp */
