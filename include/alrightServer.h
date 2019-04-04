/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/
#ifndef ALRIGHT_SERVER
#define ALRIGHT_SERVER

#include "alrightData.h"
#include "alrightSystemError.h"

#include <boost/asio.hpp>
#include <iostream>

namespace alright {

template <typename serverType>
class alrightServer {
public:
  explicit alrightServer(serverEndpointData aData)
  : mData(aData)
  , mIOcontext()
  , mEndpoint(boost::asio::ip::address_v4::any(), mData.portNumber)
  , mAcceptor(mIOcontext, mEndpoint) {
    listenForConnection();
  }

  ~alrightServer() {
    mIOcontext.stop();
  }

  const void run() {
    mIOcontext.run();
  }


private:
  void listenForConnection() {
    auto lConnection = std::make_shared<serverType>(mData, mIOcontext);
    mAcceptor.async_accept(*lConnection->getSocket(), 
              [this, lConnection](const boost::system::error_code& error) { 
                              handleConnection(error, lConnection);
                            });

  }

  void handleConnection(const boost::system::error_code& errCode,
	std::shared_ptr<serverType> lConnection) {
    if (!errCode) {
      lConnection->startHandling();
      mConnections.push_back(lConnection);
    } else {
      systemError(errCode);
    }
    listenForConnection();
  }

  serverEndpointData mData;
  boost::asio::io_context mIOcontext;
  boost::asio::ip::tcp::endpoint mEndpoint;
  boost::asio::ip::tcp::acceptor mAcceptor;
  std::vector<std::shared_ptr<serverType>> mConnections;
};

} // namespace alright

#endif
