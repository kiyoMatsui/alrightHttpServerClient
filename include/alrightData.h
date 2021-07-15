/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/
#ifndef ALRIGHT_DATA
#define ALRIGHT_DATA

#include <string>
#include <utility>

namespace alright {

class headerField {
 public:
  explicit headerField(std::string aName, std::string aValue) : name(std::move(aName)), value(std::move(aValue)) {}

 private:
  std::string name;
  std::string value;
};

class serverEndpointData {
 public:
  explicit serverEndpointData(uint16_t aPortNumber, std::string aServerPath)
      : portNumber(aPortNumber), serverPath(std::move(aServerPath)) {}
  const uint16_t portNumber;
  const std::string serverPath;
};

class clientEndpointData {
 public:
  explicit clientEndpointData(uint16_t aPN, std::string aAddress, std::string aRequestMethod, std::string aRequestURI,
                              std::string aRequestHttpVersion)
      : portNumber(aPN),
        address(std::move(aAddress)),
        requestMethod(std::move(aRequestMethod)),
        requestURI(std::move(aRequestURI)),
        requestHttpVersion(std::move(aRequestHttpVersion)) {}
  const uint16_t portNumber;
  const std::string address;
  const std::string requestMethod;
  const std::string requestURI;
  const std::string requestHttpVersion;
};

}  // namespace alright

#endif
