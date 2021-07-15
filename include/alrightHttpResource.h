/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/
#ifndef ALRIGHT_HTTPRESOURCE
#define ALRIGHT_HTTPRESOURCE

#include <boost/asio.hpp>
#include <filesystem>
#include <fstream>

namespace alright {

int getHttpResource(std::string aPath, std::string& aRequestMethod, std::string& aResource, std::string& aBody) {
  if (aPath.back() == '/') {
    aPath.pop_back();
  }

  std::string lResourceFullPath;
  if (aResource == "/" || aResource.empty()) {
    lResourceFullPath = aPath + "/index.html";
  } else {
    lResourceFullPath = aPath + aResource;
  }

  if (!std::filesystem::exists(lResourceFullPath)) {
    aBody = "404 Not found\r\n";
    return 404;
  }

  std::ifstream lResourcefstream(lResourceFullPath);

  if (!lResourcefstream.is_open()) {
    aBody = "500 Internal server error\r\n";
    return 500;
  }

  if (aRequestMethod.compare("GET")) {
    aBody = "501 Method Not implemented\r\n";
    return 501;
  }

  while (!lResourcefstream.eof()) {
    std::string lLine;
    std::getline(lResourcefstream, lLine);
    aBody += lLine;
  }
  return 200;
}

}  // namespace alright

#endif
