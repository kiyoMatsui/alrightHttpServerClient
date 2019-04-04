/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/
#ifndef ALRIGHT_HTTPCONN
#define ALRIGHT_HTTPCONN

#include "alrightData.h"
#include "alrightHttpResource.h"

#include <boost/asio.hpp>

#include <fstream>
#include <atomic>
#include <thread>
#include <iostream>

namespace alright {

class httpConnection {
 public:
  httpConnection(serverEndpointData aData,
                 boost::asio::io_context& aIOcontext)
      : mData(aData),
        mSocket(std::make_shared<boost::asio::ip::tcp::socket>(aIOcontext)) {
  };

  std::shared_ptr<boost::asio::ip::tcp::socket> getSocket() {
     return mSocket;
  }

  void startHandling() {
    mSocket->async_read_some(boost::asio::buffer(mRequest),
                           [this](const boost::system::error_code& errCode,
                                  std::size_t transferred_size_t) {
                                  requestReceived(errCode, transferred_size_t);
                            });
  }
  
 private:
  int processRequest(std::string& aHtml) {
    std::string str(std::begin(mRequest), std::end(mRequest));
    std::size_t justReqlineAndHeaderPos = str.find("\r\n\r\n"); //ignore optional msg
    std::string justReqlineAndHeader = str.substr(0, justReqlineAndHeaderPos); //include last \r
    std::istringstream receivedStream(justReqlineAndHeader);
    //
    // the request line
    //
    std::string requestMethod, requestURI, requestHttpVersion;
    receivedStream >> requestMethod;
    receivedStream >> requestURI;
    receivedStream >> requestHttpVersion;
    // carriageReturnLineFeed;
    receivedStream.get(); receivedStream.get(); 
    //
    // the request headers
    //
    while (!receivedStream.eof()) {
      std::string aName, aValue;
      std::getline(receivedStream, aName, ':');
      receivedStream.get(); // white space
      std::getline(receivedStream, aValue, '\r');
      receivedStream.get(); // \n
      
      std::vector<headerField> lRequestHeaders;      
      lRequestHeaders.emplace_back(aName, aValue);
    }
    return alright::getHttpResource(mData.serverPath,requestMethod,requestURI, aHtml);
  }
  
  void requestReceived(const boost::system::error_code& errCode,
                       std::size_t transferred_size_t) {
    if (errCode) {
      systemError(errCode);
      return;
    }
    std::vector<boost::asio::const_buffer> asioResponseBuffers;
    int lStatusCode = processRequest(lHtml);    
    lResponseStatusLine = "HTTP/1.1 " +
                          std::to_string(lStatusCode) +
                          " status line end" + "\r\n";
    lResponseHeaders = std::string("Server: ") +
                       "alrightServer" +
                       "\r\n" +
                       std::string("content-length") +
                       ": " +
                       std::to_string(lHtml.size()) +
                       "\r\n" +
                       std::string("content-type") +
                       ": " +
                       std::string("html") +
                       "\r\n\r\n";
                                   
    asioResponseBuffers.push_back(boost::asio::buffer(lResponseStatusLine));
    asioResponseBuffers.push_back(boost::asio::buffer(lResponseHeaders));
    asioResponseBuffers.push_back(boost::asio::buffer(lHtml, lHtml.size()));
    mSocket->async_write_some(asioResponseBuffers,
			[this](
			const boost::system::error_code& errCode,
			std::size_t transferred_size_t) {
			shutdown(errCode, transferred_size_t);
                        });
    return;
  }

  void shutdown(const boost::system::error_code& errCode,
		std::size_t transferred_size_t) {
    if (errCode) {
      systemError(errCode);
    }
    mSocket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
  }

 private:
  serverEndpointData mData;
  std::shared_ptr<boost::asio::ip::tcp::socket> mSocket;
  std::array<char,8192> mRequest;
 public:
  std::string lHtml; //buffer requires persistent data
  std::string lResponseStatusLine;
  std::string lResponseHeaders;
};


} // namespace alright

#endif
