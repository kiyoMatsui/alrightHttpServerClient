/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/
#ifndef ALRIGHT_SERVER
#define ALRIGHT_SERVER

#include <boost/asio.hpp>
#include <iostream>

#include "alrightData.h"
#include "alrightSystemError.h"

namespace alright {

template <typename serverType>
class alrightServer {
 public:
  explicit alrightServer(serverEndpointData aData)
      : mData(aData),
        mIOcontext(),
        mEndpoint(boost::asio::ip::address_v4::any(), (unsigned short)mData.portNumber),
        mAcceptor(mIOcontext, mEndpoint) {
    listenForConnection();
  }

  ~alrightServer() { mIOcontext.stop(); }

  void run() { mIOcontext.run(); }

 private:
  void listenForConnection() {
    auto lConnection = std::make_shared<serverType>(mData, mIOcontext);
    mAcceptor.async_accept(*lConnection->getSocket(), [this, lConnection](const boost::system::error_code& error) {
      handleConnection(error, lConnection);
    });
  }

  void handleConnection(const boost::system::error_code& errCode, std::shared_ptr<serverType> aConnection) {
    if (!errCode) {
      aConnection->startHandling();
    } else {
      systemError(errCode);
    }
    listenForConnection();
  }

  serverEndpointData mData;
  boost::asio::io_context mIOcontext;
  boost::asio::ip::tcp::endpoint mEndpoint;
  boost::asio::ip::tcp::acceptor mAcceptor;
};

}  // namespace alright

#endif
