/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/
#ifndef ALRIGHT_CLIENT
#define ALRIGHT_CLIENT

#include "alrightData.h"
#include "alrightHttpRequest.h"
#include <boost/asio.hpp>

namespace alright {

class alrightClient {
 public:
  explicit alrightClient()
      : mIOcontext() {
  }

  ~alrightClient() {
    mIOcontext.stop();
  }

  void run() {
    mIOcontext.run();
  }

  void request(clientEndpointData aData) {
    auto request = std::make_shared<alrightHttpRequest>(aData,mIOcontext);
    request->resolveQuery();
  }

 private:
  boost::asio::io_context mIOcontext;
};

} // namespace alright

#endif
