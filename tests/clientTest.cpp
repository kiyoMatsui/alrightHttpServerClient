/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/

#include <iostream>
#include "catch2/catch.hpp"

#include "alrightClient.h"

TEST_CASE("client") {
  alright::clientEndpointData lData2(9999,
                                     "localhost",
                                     "GET",
                                     "/",
                                     "HTTP/1.1");
  alright::alrightClient client{};
  client.request(lData2);
  client.run();
  if (client.getLastRequest().first) {
    std::cout.write(client.getLastRequest().second->mBytes.data(),
                    client.getLastRequest().second->mTransferred_size_t);
    std::cout << std::endl;
  }
  client.cleanUp();
}


