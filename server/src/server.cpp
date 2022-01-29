#include "../include/server.h"

server::server(int port)
    : port_(port), acceptor_(io, tcp::endpoint(tcp::v4(), port_)) {}

int server::run() {
  try {
    start_accept();
    io.run();
  } catch (std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return -1;
  }
  return 0;
}

void server::broadcast(const std::string& message) {
  for (auto& connection : connections_) {
    connection->post(message);
  }
}

void server::start_accept() {
  socket_.emplace(io);
  acceptor_.async_accept(*socket_, [this](error_code ec) {
    auto connection = connection::create(std::move(*socket_));
    if (on_join) on_join(connection);
    connections_.insert(connection);
    if (!ec) {
      connection->start(
          [this](const std::string& msg) {
            if (on_cmsg) on_cmsg(msg);
          },
          [&, weak = std::weak_ptr(connection)] {
            if (auto shared = weak.lock();
                shared && connections_.erase(shared)) {
              if (on_leave) on_leave(shared);
            }
          });
    }
    start_accept();
  });
}
