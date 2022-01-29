#ifndef CONNECTION_H
#define CONNECTION_H
#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <queue>

namespace net = boost::asio;
class connection : public std::enable_shared_from_this<connection> {
  using tcp = net::ip::tcp;
  using error_code = boost::system::error_code;
  using msg_h = std::function<void(std::string)>;
  using err_h = std::function<void()>;

 public:
  using pointer = std::shared_ptr<connection>;
  static pointer create(tcp::socket&& socket) {
    return pointer(new connection(std::move(socket)));
  }
  net::ip::tcp::socket& socket();

  const std::string& get_username() const;

  void start(msg_h&& msg_h, err_h&& err_h);

  void post(const std::string& message);

 private:
  explicit connection(tcp::socket&& socket);
  void read();
  void on_read(error_code ec, std::size_t bytes);
  void write();
  void on_write(error_code ec, std::size_t bytes);

 private:
  tcp::socket socket_;
  std::string username_;
  std::queue<std::string> out_mesgs_;
  net::streambuf buffer_{65536};
  msg_h msg_h_;
  err_h err_h_;
};
#endif  // CONNECTION_H
