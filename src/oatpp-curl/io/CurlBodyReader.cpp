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

#include "CurlBodyReader.hpp"

#include <chrono>

namespace oatpp { namespace curl { namespace io {
  
/*
 * This callback may be called several times during one non-blocking perform.
 * So check for (instance->m_position != 0). m_position == 0 means it was called multiple times in a row
 * it can happen if response in chunked encoded
 */
size_t CurlBodyReader::writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  CurlBodyReader* instance = static_cast<CurlBodyReader*>(userdata);
  
  if(instance->m_position != 0) {
    if(instance->m_position != instance->m_buffer.getSize()){
      throw std::runtime_error("[oatpp::curl::CurlBodyReader::writeCallback(...)]: Invalid state.");
    }
    instance->m_buffer.clear();
    instance->m_position = 0;
  }
  return instance->m_buffer.write(ptr, size * nmemb);
}
  
data::v_io_size CurlBodyReader::read(void *data, data::v_io_size count) {
  data::v_io_size readCount;
  while ((readCount = readNonBlocking(data, count)) == oatpp::data::IOError::WAIT_RETRY) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return readCount;
}
  
data::v_io_size CurlBodyReader::readNonBlocking(void *data, data::v_io_size count) {
  
  data::v_io_size availableBytes = getAvailableBytesCount();
  
  if(availableBytes == 0) {
    
    int still_running = 1;
    curl_multi_perform(m_handles->getMultiHandle(), &still_running);
    
    availableBytes = getAvailableBytesCount();
    
    if(availableBytes == 0) {
      
      if(still_running) {
        return oatpp::data::IOError::WAIT_RETRY;
      } else {
        return oatpp::data::IOError::BROKEN_PIPE;
      }
      
    }
    
  }
  
  auto readCount = m_buffer.readSubstring(data, m_position, count);
  m_position += readCount;
  return readCount;
  
}
  
data::v_io_size CurlBodyReader::getAvailableBytesCount() {
  return m_buffer.getSize() - m_position;
}
  
}}}
