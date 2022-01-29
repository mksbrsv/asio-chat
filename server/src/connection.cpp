#include "../include/connection.h"

boost::asio::ip::tcp::socket& connection::socket() { return socket_; }

const std::string& connection::get_username() const { return username_; }

void connection::start(msg_h&& msg_h, err_h&& err_h) {
  msg_h_ = std::move(msg_h);
  err_h_ = std::move(err_h);
  read();
}

void connection::post(const std::string& message) {
  bool queue_idle = out_mesgs_.empty();
  out_mesgs_.push(message);
  if (queue_idle) write();
}

connection::connection(tcp::socket&& socket) : socket_(std::move(socket)) {
  std::stringstream name;
  name << socket_.remote_endpoint();
  username_ = name.str();
}
void connection::read() {
  net::async_read_until(
      socket_, buffer_, "\n",
      [self = shared_from_this()](error_code ec, std::size_t bytes) {
        self->on_read(ec, bytes);
      });
}
void connection::on_read(error_code ec, std::size_t bytes) {
  if (ec) {
    socket_.close(ec);
    err_h_();
    return;
  }
  std::stringstream msg;
  msg << username_ << ": " << std::istream(&buffer_).rdbuf();
  msg_h_(msg.str());
  read();
}
void connection::write() {
  net::async_write(
      socket_, net::buffer(out_mesgs_.front()),
      [self = shared_from_this()](error_code ec, std::size_t bytes) {
        self->on_write(ec, bytes);
      });
}
void connection::on_write(error_code ec, std::size_t bytes) {
  if (ec) {
    socket_.close(ec);
    err_h_();
    return;
  }
  out_mesgs_.pop();
  if (!out_mesgs_.empty()) write();
}
