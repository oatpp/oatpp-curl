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

#include "CurlReader.hpp"

#include <chrono>

namespace oatpp { namespace curl {
  
size_t CurlReader::writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  OATPP_LOGD("curl", "reader::callback(data*, size=%d, nmemb=%d)", size, nmemb);
  CurlReader* instance = static_cast<CurlReader*>(userdata);
  instance->m_buffer.clear();
  instance->m_position = 0;
  return instance->m_buffer.write(ptr, size * nmemb);
}
  
os::io::Library::v_size CurlReader::read(void *data, os::io::Library::v_size count) {
  os::io::Library::v_size readCount;
  while ((readCount = readNonBlocking(data, count)) == oatpp::data::stream::Errors::ERROR_IO_RETRY) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return readCount;
}
  
os::io::Library::v_size CurlReader::readNonBlocking(void *data, os::io::Library::v_size count) {
  
  os::io::Library::v_size availableBytes = m_buffer.getSize() - m_position;
  
  if(availableBytes == 0) {
    
    int still_running = 1;
    curl_multi_perform(m_handles->getMultiHandle(), &still_running);
    
    availableBytes = m_buffer.getSize() - m_position;
    
    if(availableBytes == 0) {
      
      if(still_running) {
        return oatpp::data::stream::Errors::ERROR_IO_RETRY;
      } else {
        return oatpp::data::stream::Errors::ERROR_IO_PIPE;
      }
      
    }
    
  }
  
  auto readCount = m_buffer.readSubstring(data, m_position, count);
  m_position += readCount;
  return readCount;
  
}
  
}}
