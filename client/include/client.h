#ifndef CLIENT_H
#define CLIENT_H
#include <boost/asio.hpp>
#include <functional>
#include <queue>

namespace net = boost::asio;
class client {
  using tcp = net::ip::tcp;
  using msg_h = std::function<void(std::string)>;
  using error_code = boost::system::error_code;

 public:
  client(const std::string& address, int port);
  void run();
  void stop();
  void post(const std::string& msg);
  template <class Function>
  void reg_msg_h(Function&& f) {
    on_msg = std::move(f);
  }

 private:
  void read();
  void on_read(error_code ec, std::size_t bytes);
  void write();
  void on_write(error_code ec, std::size_t bytes);

 private:
  // Fields
  net::io_context io_{};
  tcp::socket socket_;
  tcp::resolver::results_type eps_;
  net::streambuf buffer_{65536};
  std::queue<std::string> out_msgs_{};
  // Event handlers
  msg_h on_msg;
};

#endif  // CLIENT_H
