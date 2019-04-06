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

#include "BodyOutputStream.hpp"

namespace oatpp { namespace curl { namespace io {
  
BodyOutputStream::BodyOutputStream(const std::shared_ptr<CurlBodyWriter> writer, bool nonBlocking)
  : m_writer(writer)
  , m_nonBlocking(nonBlocking)
{}

data::v_io_size BodyOutputStream::write(const void *data, data::v_io_size count) {
  if(m_nonBlocking) {
    return m_writer->writeNonBlocking(data, count);
  } else {
    return m_writer->write(data, count);
  }
}
  
}}}
