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

#ifndef oatpp_curl_BodyInputStream_hpp
#define oatpp_curl_BodyInputStream_hpp

#include "CurlBodyReader.hpp"

namespace oatpp { namespace curl { namespace io {

/**
 * Wrapper over &id:oatpp::curl::io::CurlBodyReader; providing &id:oatpp::data::stream::InputStream; interface.
 */
class BodyInputStream : public oatpp::data::stream::InputStream {
private:
  std::shared_ptr<CurlBodyReader> m_reader;
  oatpp::data::stream::IOMode m_ioMode;
public:

  /**
   * Constructor.
   * @param reader
   * @param ioMode
   */
  BodyInputStream(const std::shared_ptr<CurlBodyReader> reader, oatpp::data::stream::IOMode ioMode);

  /**
   * Read data from stream. Implementation of &id:oatpp::data::stream::InputStream::read; method.
   * @param data - buffer to read data to.
   * @param count - buffer size.
   * @return - &id:oatpp::data::v_io_size;.
   */
  data::v_io_size read(void *data, data::v_io_size count) override;

  /**
   * Implementation of InputStream must suggest async actions for I/O results.
   * Suggested Action is used for scheduling coroutines in async::Executor.
   * @param ioResult - result of the call to &l:InputStream::read ();.
   * @return - &id:oatpp::async::Action;.
   */
  oatpp::async::Action suggestInputStreamAction(data::v_io_size ioResult) override;

  /**
   * Set stream I/O mode.
   * @throws
   */
  void setInputStreamIOMode(oatpp::data::stream::IOMode ioMode) override;

  /**
   * Get stream I/O mode.
   * @return
   */
  oatpp::data::stream::IOMode getInputStreamIOMode() override;
  
};
  
}}}

#endif /* oatpp_curl_BodyInputStream_hpp */
