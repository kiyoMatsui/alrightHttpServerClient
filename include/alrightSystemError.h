/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/
#ifndef ALRIGHT_SYSTEMERROR
#define ALRIGHT_SYSTEMERROR

#include <iostream>
#include <boost/asio.hpp>

namespace alright {

void systemError (const boost::system::error_code& ec) {
  std::cout << "Boost system error code has occured! " << ec.message() << "\n";
};

} // namespace alright

#endif
