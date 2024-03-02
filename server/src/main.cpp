#include "server.hpp"

auto main(int argc, char const *const *argv) -> int {
  Server& s = Server::get_instance();  
  s.run();
  return 0;
}