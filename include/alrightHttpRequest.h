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
#include <fstream>
#include <streambuf>
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
      //std::ifstream file("raw.txt");
      //std::string requestString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); //remember to add a extra carriage return
      std::string requestString = mData.requestMethod + " " + mData.requestURI
                    + " " + mData.requestHttpVersion + "\r\nHost: "
                                + mData.address + ":" + std::to_string(mData.portNumber) +
                                "\r\nUser-Agent: " + "AlrightClient" +
                                "\r\nAccept: " + "*/*"  + "\r\n\r\n";
      
      boost::asio::async_write(mSocket, boost::asio::buffer(requestString),
                               [sharedThis = shared_from_this()](
                                   const boost::system::error_code& errCode,
                                   std::size_t aTransferred_size_t_4Signature) {
                                 sharedThis->readResponse(errCode);
                                 }); 
    } else {
      systemError(errCode);
    }
  }
    
  void readResponse(const boost::system::error_code& errCode) { 
    if (!errCode) { 
      boost::asio::async_read(mSocket, mResponse, boost::asio::transfer_at_least(1),
                                         [sharedThis = shared_from_this()](
                                  const boost::system::error_code& errCode,
                                   std::size_t aTransferred_size_t_4Signature) {
                                               sharedThis->endHandling(errCode);
                                }); 
    } else {
      systemError(errCode);
    }
  }

  void endHandling(const boost::system::error_code& errCode ) { 
    if (!errCode ) {
      std::cout << &mResponse; 
      // continue untill EOF
      boost::asio::async_read(mSocket, mResponse, boost::asio::transfer_at_least(1),
      [sharedThis = shared_from_this()](const boost::system::error_code& errCode,
                                        std::size_t aTransferred_size_t_4Signature) {                     
                                sharedThis->endHandling(errCode);             
                              });     
    } else if(errCode != boost::asio::error::eof) {
      systemError(errCode);
    } 
  }
  //mSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
  //mSocket.close();

  clientEndpointData mData;
  boost::asio::ip::tcp::endpoint mEndpoint;
  boost::asio::ip::tcp::resolver mResolver;
  boost::asio::ip::tcp::socket mSocket;
  boost::asio::streambuf mResponse;
};

} // namespace alright

#endif
