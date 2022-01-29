#include "../include/server.h"

int main(int argc, char** argv) {
  int port = 0;
  if (argc == 1) {
    std::cout << "Usage ./server <port>\n";
    return -1;
  }
  if (argc > 1) port = std::stoi(argv[1]);
  server serv(port);
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
