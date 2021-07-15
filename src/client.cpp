#include <iostream>

#include "alrightClient.h"

static constexpr uint16_t myport = 443;

int main(int argc, char* argv[]) {
  alright::clientEndpointData lData2(myport, "www.google.com", "GET", "/", "HTTP/1.1");

  alright::alrightClient client{};
  client.request(lData2);
  client.run();
}
