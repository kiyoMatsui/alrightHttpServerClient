/*-------------------------------*\
Copyright 2019 Kiyo Matsui
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/
\*-------------------------------*/
#ifndef ALRIGHT_DATA
#define ALRIGHT_DATA

namespace alright {

class headerField {
 public:
  explicit headerField(std::string aName, std::string aValue) : name(aName), value(aValue) {}

 private:
  std::string name;
  std::string value;
};

class serverEndpointData {
 public:
  explicit serverEndpointData(short unsigned int aPortNumber, std::string aServerPath)
      : portNumber(aPortNumber), serverPath(aServerPath) {}
  const short unsigned int portNumber;
  const std::string serverPath;
};

class clientEndpointData {
 public:
  explicit clientEndpointData(short unsigned int aPN, std::string aAddress, std::string aRequestMethod,
                              std::string aRequestURI, std::string aRequestHttpVersion)
      : portNumber(aPN),
        address(aAddress),
        requestMethod(aRequestMethod),
        requestURI(aRequestURI),
        requestHttpVersion(aRequestHttpVersion) {}
  const short unsigned int portNumber;
  const std::string address;
  const std::string requestMethod;
  const std::string requestURI;
  const std::string requestHttpVersion;
};

}  // namespace alright

#endif
