/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/

#include "app.h"

#include <boost/asio.hpp>
#include <filesystem>

#include "alrightClient.h"
#include "alrightData.h"
#include "alrightHttpConnection.h"
#include "alrightServer.h"
#include "appinfo.h"

static constexpr uint16_t myport = 9999;

int App::run(int argc, char** argv) const {
  alright::serverEndpointData lData(myport, std::string(std::filesystem::current_path().u8string() + "/webSrv"));

  alright::alrightServer<alright::httpConnection> srv(lData);
  srv.run();

  return 0;
}

string App::getProjectName() const {
  return APPLICATION_NAME;
}

string App::getProjectVersion() const {
  return APPLICATION_VERSION_STRING;
}
