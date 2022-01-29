#include <iostream>
#include <thread>

#include "../include/client.h"

int main(int argc, char** argv) {
  std::string host = "";
  int port = 0;
  if (argc == 1) {
    std::cout << "Usage: ./client <host> <port>";
    return -1;
  }
  if (argc > 2) {
    host = argv[1];
    port = std::stoi(argv[2]);
  }
  client client(host, port);
  client.reg_msg_h([](const std::string& msg) { std::cout << msg; });

  std::thread t{[&client] { client.run(); }};
  while (true) {
    std::string msg;
    std::getline(std::cin, msg);
    if (msg == "\\q") break;
    msg += "\n";
    client.post(msg);
  }
  client.stop();
  t.join();
}
