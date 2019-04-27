/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/
#ifndef ALRIGHT_HTTPREQ
#define ALRIGHT_HTTPREQ

#include "alrightData.h"
#include "alrightSystemError.h"

#include <boost/asio.hpp>
#include <iostream>

namespace alright {

class alrightHttpRequest : public std::enable_shared_from_this<alrightHttpRequest> {
 public:
  explicit alrightHttpRequest(clientEndpointData aData,
                              boost::asio::io_context& mIOcontext)
  : mData(aData)
  , mEndpoint(boost::asio::ip::address_v4::any(), mData.portNumber)
  , mResolver(mIOcontext)
  , mSocket(mIOcontext) {
  }
  
  void resolveQuery() {
    //endpoints
    boost::asio::ip::tcp::resolver::query queryToResolve(mData.address,
                                            std::to_string(mData.portNumber),
                                            boost::asio::ip::tcp::resolver::query::numeric_service);
    //numeric_service port should be numeric string and not a "name" like http... i think  
    mResolver.async_resolve(queryToResolve,
                            [sharedThis = shared_from_this()](
                                const boost::system::error_code& errCode,
                                boost::asio::ip::tcp::resolver::iterator iterator) {
                              sharedThis->connectToSocket(errCode, iterator);
                            });
  }
  
 private:
  void connectToSocket(const boost::system::error_code& errCode,
                     boost::asio::ip::tcp::resolver::iterator iterator) {
    if (!errCode) {
      mSocket.async_connect(*iterator,
                            [sharedThis = shared_from_this()](
                                const boost::system::error_code& errCode) {
                              sharedThis->writeToSocket(errCode);
                            });
    } else {
      systemError(errCode);
    }
  }

  void writeToSocket(const boost::system::error_code& errCode) {
    if (!errCode) {
      std::string requestString = mData.requestMethod + " " + mData.requestURI
                      + " " + mData.requestHttpVersion + "\r\nHost: "
                      + mData.address  + "\r\n\r\n";
      mSocket.async_write_some(boost::asio::buffer(requestString),
                               [sharedThis = shared_from_this()](
                                   const boost::system::error_code& errCode,
                                   std::size_t bytes_transferred) {
                                 sharedThis->readResponse(errCode, bytes_transferred);
                               });
    } else {
      systemError(errCode);
    }
  }
    
  void readResponse(const boost::system::error_code& errCode,
                   std::size_t bytes_transferred) { 
    if (!errCode) {
      mSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
      mSocket.async_read_some(boost::asio::buffer(mBytes),
                              [sharedThis = shared_from_this()](
                                  const boost::system::error_code& errCode,
                                  std::size_t bytes_transferred) {
                                sharedThis->endHandling(errCode, bytes_transferred);
                              });
    } else {
      systemError(errCode);
    }
  }

  void endHandling(const boost::system::error_code& errCode,
                   std::size_t transferred_size_t) { 
    if (!errCode) {
      mTransferred_size_t = transferred_size_t;
      mDone = true;
      std::cout.write(mBytes.data(),mTransferred_size_t);
      std::cout << std::endl;
    } else {
      systemError(errCode);
    }
  }

  clientEndpointData mData;
  boost::asio::ip::tcp::endpoint mEndpoint;
  boost::asio::ip::tcp::resolver mResolver;
  boost::asio::ip::tcp::socket mSocket;
  std::size_t mTransferred_size_t;
  std::array<char, 4096> mBytes;
 public:
  bool mDone = false;
};

} // namespace alright

#endif
