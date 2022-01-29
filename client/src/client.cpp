#include "../include/client.h"

client::client(const std::string& address, int port) : socket_(io_) {
  tcp::resolver resolver(io_);
  eps_ = resolver.resolve(address, std::to_string(port));
}
void client::run() {
  net::async_connect(
      socket_, eps_,
      [this](boost::system::error_code ec, net::ip::tcp::endpoint ep) {
        if (!ec) read();
      });
  io_.run();
}
void client::stop() {
  error_code ec;
  socket_.close(ec);
  if (ec) {
  }
}
void client::post(const std::string& msg) {
  bool queue_idle = out_msgs_.empty();
  out_msgs_.push(msg);
  if (queue_idle) write();
}

void client::read() {
  net::async_read_until(
      socket_, buffer_, "\n",
      [this](error_code ec, std::size_t bytes) { on_read(ec, bytes); });
}
void client::on_read(error_code ec, std::size_t bytes) {
  if (ec) {
    stop();
    return;
  }
  std::stringstream msg;
  msg << std::istream(&buffer_).rdbuf();
  on_msg(msg.str());
  read();
}
void client::write() {
  net::async_write(
      socket_, net::buffer(out_msgs_.front()),
      [this](error_code ec, std::size_t bytes) { on_write(ec, bytes); });
}
void client::on_write(error_code ec, std::size_t bytes) {
  if (ec) {
    stop();
    return;
  }
  out_msgs_.pop();
  if (!out_msgs_.empty()) write();
}
