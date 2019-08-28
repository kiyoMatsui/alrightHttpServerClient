/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/
#ifndef ALRIGHT_HTTPSREQ
#define ALRIGHT_HTTPSREQ

#include "alrightData.h"
#include "alrightSystemError.h"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <fstream>
#include <streambuf>
#include <iostream>

using boost::asio::ip::tcp;
using std::placeholders::_1;
using std::placeholders::_2;
namespace alright {

class alrightHttpsRequest : public std::enable_shared_from_this<alrightHttpsRequest> {
 public:
  explicit alrightHttpsRequest(clientEndpointData aData,
                              boost::asio::io_context& mIOcontext)
  : mData(aData)
  , mEndpoint(boost::asio::ip::address_v4::any(), mData.portNumber)
  , mResolver(mIOcontext)
  , ctx(boost::asio::ssl::context::sslv23)
  , mSocket(mIOcontext, ctx){
    ctx.load_verify_file("ssl/ca.pem");
    mSocket.set_verify_mode(boost::asio::ssl::verify_peer);
    mSocket.set_verify_callback([this](bool preverified,
                                       boost::asio::ssl::verify_context& aCtx)->bool {
                                  return verify_certificate(preverified, aCtx);
                                  }); 
        }
  
  bool verify_certificate(bool preverified, boost::asio::ssl::verify_context& ctx)
  {
    // The verify callback can be used to check whether the certificate that is
    // being presented is valid for the peer. For example, RFC 2818 describes
    // the steps involved in doing this for HTTPS. Consult the OpenSSL
    // documentation for more details. Note that the callback is called once
    // for each certificate in the certificate chain, starting from the root
    // certificate authority.

    // In this example we will simply print the certificate's subject name.
    char subject_name[256];
    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
    std::cout << "Verifying " << subject_name << "\n";

    return preverified;
  }
  
  void resolveQuery() {
    //endpoints
    boost::asio::ip::tcp::resolver::query queryToResolve(mData.address,
                                            std::to_string(mData.portNumber),
                                            boost::asio::ip::tcp::resolver::query::numeric_service);
    mResolver.async_resolve(queryToResolve,
                            [sharedThis = shared_from_this()](
                                const boost::system::error_code& errCode,
                                boost::asio::ip::tcp::resolver::results_type mRes) {
                              sharedThis->connectToSocket(errCode, mRes);
                            });
  }
  
 private:
  void connectToSocket(const boost::system::error_code& errCode,
                     boost::asio::ip::tcp::resolver::results_type mRes) {
    if (!errCode) { 
      boost::asio::async_connect(mSocket.lowest_layer(), mRes,
                            [sharedThis = shared_from_this()](
                                const boost::system::error_code& errCode,
                                const tcp::endpoint& /*endpoint*/) {
                              sharedThis->handshake(errCode);
                              }); 
    } else {
      systemError(errCode);
    }
  }
  
  void handshake(const boost::system::error_code& errCode) {
    if (!errCode) { 
      mSocket.async_handshake(boost::asio::ssl::stream_base::client,
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
  boost::asio::ssl::context ctx;
  boost::asio::ssl::stream<tcp::socket>  mSocket;
  boost::asio::streambuf mResponse;
};

} // namespace alright

#endif
