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

#ifndef oatpp_curl_RequestExecutor_hpp
#define oatpp_curl_RequestExecutor_hpp

#include "./io/BodyDecoder.hpp"

#include "oatpp/web/client/RequestExecutor.hpp"

namespace oatpp { namespace curl {
  
class RequestExecutor : public oatpp::web::client::RequestExecutor {
private:
  oatpp::String m_baseUrl;
  std::shared_ptr<io::BodyDecoder> m_bodyDecoder;
  bool m_verbose;
public:
  
  RequestExecutor(const oatpp::String& baseUrl, bool verbose = false)
    : m_baseUrl(baseUrl)
    , m_bodyDecoder(std::make_shared<io::BodyDecoder>())
    , m_verbose(verbose)
  {}
  
  static std::shared_ptr<RequestExecutor> createShared(const oatpp::String& baseUrl, bool verbose = false) {
    return std::make_shared<RequestExecutor>(baseUrl, verbose);
  }
  
  std::shared_ptr<ConnectionHandle> getConnection() override {
    throw std::runtime_error("[oatpp::curl::RequestExecutor::getConnection()]: Error. This call is not implemented yet");
  }
  
  Action getConnectionAsync(oatpp::async::AbstractCoroutine* parentCoroutine, AsyncConnectionCallback callback) override {
    throw std::runtime_error("[oatpp::curl::RequestExecutor::getConnectionAsync(...)]: Error. This call is not implemented yet");
  }
  
  std::shared_ptr<Response> execute(const String& method,
                                    const String& path,
                                    const std::shared_ptr<Headers>& userDefinedHeaders,
                                    const std::shared_ptr<Body>& body,
                                    const std::shared_ptr<ConnectionHandle>& connectionHandle = nullptr) override;
  
  Action executeAsync(oatpp::async::AbstractCoroutine* parentCoroutine,
                      AsyncCallback callback,
                      const String& method,
                      const String& path,
                      const std::shared_ptr<Headers>& headers,
                      const std::shared_ptr<Body>& body,
                      const std::shared_ptr<ConnectionHandle>& connectionHandle = nullptr) override;
  
};
  
}}

#endif /* oatpp_curl_RequestExecutor_hpp */
