#include "../include/server.h"

int main() {
  server serv(8080);
  serv.reg_join_h([](connection::pointer client) {
    std::cout << "User has joined the server: " << client->get_username()
              << std::endl;
  });
  serv.reg_leave_h([](connection::pointer client) {
    std::cout << "User has left the server: " << client->get_username()
              << std::endl;
  });
  serv.reg_cmsg_h(
      [&serv](const std::string& message) { serv.broadcast(message); });
  serv.run();
}
