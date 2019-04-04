#ifndef SRC_APP_H_
#define SRC_APP_H_

#include <string>

using namespace std;

class App {

public:
  int run(int argc, char** argv) const;

  string getProjectName() const;
  string getProjectVersion() const;
};

#endif
