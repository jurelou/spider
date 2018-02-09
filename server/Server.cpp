#include        <iostream>
#include        "Server.hpp"

Server::Server(boost::asio::io_service& io_service, int port)
  : serverAcceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), sslContext(io_service, boost::asio::ssl::context::sslv23), input(2048), inputHandler(io_service, ::dup(STDIN_FILENO)) {
  this->sslContext.set_password_callback(boost::bind(&Server::getPasswordCallback, this));
  this->sslContext.use_certificate_chain_file("./Keys/server.crt");
  this->sslContext.use_private_key_file("./Keys/server.key", boost::asio::ssl::context::pem);
  this->sslContext.use_tmp_dh_file("./Keys/dh2048.pem");
  this->plug = Plugins::create();
  this->cmd = Cmd::create();
  this->plug->loadPlugins();
  this->handleAccept();
  this->routine();
}

Server::~Server() {}

std::string     Server::getPasswordCallback() {
  return "0xD34DB33F";
}

void            Server::routineCallback(const boost::system::error_code& error, std::size_t length) {
  if (!error) {
    std::istream is(&this->input);
    std::string   s;
    is >> s;
    if (s.length() == 0) {
      cmd->updatePool(this->clientPool);
      cmd->manageCmd(this->parse);
      this->parse.clear();
    } else {
      this->parse.push_back(s);
    }
    this->routine();
  } else {
    std::cerr << "routine: " << error.message() << std::endl;
  }

}

void            Server::handleAcceptCallback(boost::shared_ptr<SocketIO> newClient, const boost::system::error_code& error) {
  if (!error) {
    this->clientPool.push_back(newClient);
    newClient->sslHandshake();
    this->handleAccept();
  } else {
    std::cerr << error.message() << std::endl;
  }
}

void            Server::handleAccept() {
  boost::shared_ptr<SocketIO> newClient = SocketIO::create(this->serverAcceptor.get_io_service(), this->sslContext, this->plug);
  this->serverAcceptor.async_accept(newClient->getSocket(), boost::bind(&Server::handleAcceptCallback, this, newClient, boost::asio::placeholders::error));
}

void            Server::routine() {
  boost::asio::async_read_until(this->inputHandler, this->input, '\n', boost::bind(&Server::routineCallback, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}
