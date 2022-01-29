#ifndef SERVER_H
#define SERVER_H
#include <functional>
#include <optional>
#include <unordered_set>

#include "connection.h"

namespace net = boost::asio;
class server {
  using tcp = net::ip::tcp;
  using error_code = boost::system::error_code;
  using on_join_h = std::function<void(connection::pointer)>;
  using on_leave_h = std::function<void(connection::pointer)>;
  using on_cmsg_h = std::function<void(std::string)>;

 public:
  explicit server(int port);
  int run();

  void broadcast(const std::string& message);

  template <class Function>
  void reg_join_h(Function&& f) {
    on_join = std::move(f);
  }

  template <class Function>
  void reg_leave_h(Function&& f) {
    on_leave = std::move(f);
  }

  template <class Function>
  void reg_cmsg_h(Function&& f) {
    on_cmsg = std::move(f);
  }

 private:
  void start_accept();

 private:
  // Class fields
  int port_;
  net::io_context io;
  tcp::acceptor acceptor_;
  std::optional<tcp::socket> socket_;
  std::unordered_set<connection::pointer> connections_{};

  // Event handlers
  on_join_h on_join;
  on_leave_h on_leave;
  on_cmsg_h on_cmsg;
};

#endif  // SERVER_H
