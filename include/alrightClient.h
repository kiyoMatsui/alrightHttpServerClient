/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/
#ifndef ALRIGHT_CLIENT
#define ALRIGHT_CLIENT

#include <boost/asio.hpp>

#include "alrightData.h"
#include "alrightHttpsRequest.h"

namespace alright {

class alrightClient {
 public:
  explicit alrightClient() : mIOcontext() {}

  alrightClient(const alrightClient& arg1) = delete;
  alrightClient& operator=(const alrightClient& arg1) = delete;
  alrightClient(alrightClient&& other) noexcept = delete;
  alrightClient& operator=(alrightClient&& other) noexcept = delete;

  ~alrightClient() { mIOcontext.stop(); }

  void run() { mIOcontext.run(); }

  void request(clientEndpointData& aData) {
    auto request = std::make_shared<alrightHttpsRequest>(aData, mIOcontext);
    request->resolveQuery();
  }

 private:
  boost::asio::io_context mIOcontext;
};

}  // namespace alright

#endif
