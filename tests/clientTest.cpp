/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/

#include <iostream>

#include "alrightClient.h"
#include "catch2/catch.hpp"

static constexpr uint16_t myport = 443;

TEST_CASE("client") {
  alright::clientEndpointData lData2(myport, "www.google.com", "GET", "/", "HTTP/1.1");

  alright::alrightClient client{};
  client.request(lData2);
  client.run();
}
