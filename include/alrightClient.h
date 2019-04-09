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

  std::pair<bool,std::shared_ptr<alrightHttpRequest>> getLastRequest() const {
    if (mRequests.back()->mDone) {
      return std::make_pair(true, mRequests.back());
    }
    return std::make_pair(false, mRequests.back());
  }

  void request(clientEndpointData aData) {
    //mRequests.emplace_back(aData); //didn't work for some reason... #checkLater
    auto request = std::make_shared<alrightHttpRequest>(aData,mIOcontext);
    mRequests.push_back(request);
  }

  void cleanUp() {
    for(auto iter=mRequests.begin(); iter!=mRequests.end();) {
      if((*iter)->mDone == true) {
        std::swap(*iter, mRequests.back());
        mRequests.pop_back();
      } else {
        //dothings
        iter++;
      }
    }
  }
 private:
  std::vector<std::shared_ptr<alrightHttpRequest>> mRequests;
  boost::asio::io_context mIOcontext;
};

} // namespace alright

#endif
