#include <iostream>
#include <thread>

#include "../include/client.h"

int main() {
  client client("localhost", 8080);
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
