/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/

#include "app.h"
#include "appinfo.h"
#include "alrightServer.h"
#include "alrightClient.h"
#include "alrightHttpConnection.h"
#include "alrightData.h"

#include <boost/asio.hpp>


int App::run(int argc, char** argv) const {
  alright::serverEndpointData lData(9999,
  std::string("/home/cocopad/KMWORKSPACE/kMTemplate2019/webSrv"));

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


