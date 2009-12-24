#include "common.h"

#include <iostream>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "Platform.h"
#include "SocketCore.h"

int main(int argc, char* argv[]) {
  aria2::Platform platform;

#ifdef ENABLE_NLS
  // Set locale to C to prevent the messages to be localized.
  setlocale (LC_CTYPE, "C");
  setlocale (LC_MESSAGES, "C");
#endif // ENABLE_NLS

  // By default, SocketCore uses AF_UNSPEC for getaddrinfo hints to
  // resolve address. Sometime SocketCore::bind() and
  // SocketCore::establishConnection() use difference protocl family
  // and latter cannot connect to former. To avoid this situation, we
  // limit protocol family to AF_INET for unit tests.
  aria2::SocketCore::setProtocolFamily(AF_INET);
  // If AI_ADDRCONFIG is set, tests fail if IPv4 address is not
  // configured.
  aria2::setDefaultAIFlags(0);

  CppUnit::Test* suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
  CppUnit::TextUi::TestRunner runner;
  runner.addTest(suite);

  runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), std::cerr));

  // Run the tests.
  bool successfull = runner.run();

  return successfull ? 0 : 1;
}
