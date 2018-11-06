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

#include "oatpp/web/client/RequestExecutor.hpp"

namespace oatpp { namespace curl {
  
class RequestExecutor : public oatpp::web::client::RequestExecutor {
public:
  std::shared_ptr<Response> execute(const String& method,
                                    const String& path,
                                    const std::shared_ptr<Headers>& userDefinedHeaders,
                                    const std::shared_ptr<Body>& body) override;
  
  virtual Action executeAsync(oatpp::async::AbstractCoroutine* parentCoroutine,
                              AsyncCallback callback,
                              const String& method,
                              const String& path,
                              const std::shared_ptr<Headers>& headers,
                              const std::shared_ptr<Body>& body) override {
    OATPP_LOGD("Executor", "AsyncCall::");
    return Action::_FINISH;
  }
  
};
  
}}

#endif /* oatpp_curl_RequestExecutor_hpp */
