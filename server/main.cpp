#include                  <iostream>
#include                  "Server.hpp"

int                       main(int ac, char **av) {
  boost::asio::io_service ios;
  Server                  serv(ios, 1337);

  ios.run();
  return (0);
}
