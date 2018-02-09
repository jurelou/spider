#include    <iostream>
#include    "SocketIO.hpp"

SocketIO::SocketIO(boost::asio::io_service& io_service, boost::asio::ssl::context& context, boost::shared_ptr<Plugins> plug) : socket(io_service, context), plugin(plug) {}

SocketIO::~SocketIO() {}

boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::lowest_layer_type& SocketIO::getSocket() {
  return this->socket.lowest_layer();
}

boost::shared_ptr<SocketIO>   SocketIO::create(boost::asio::io_service& io_service, boost::asio::ssl::context& context, boost::shared_ptr<Plugins> plug) {
  return boost::shared_ptr<SocketIO>(new SocketIO(io_service, context, plug));
}

void                          SocketIO::sslHandshakeCallback(const boost::system::error_code& error) {
  if (!error) {
    this->initData();
    this->receive();
  } else {
    std::cerr << error.message() << std::endl;
  }
}

const std::string&            SocketIO::getIp() {
  return this->ip;
}

const std::string&            SocketIO::getHostname() {
  return this->hostname;
}

void                          SocketIO::sslHandshake() {
  this->socket.async_handshake(boost::asio::ssl::stream_base::server, boost::bind(&SocketIO::sslHandshakeCallback, this, boost::asio::placeholders::error));
}

void                          SocketIO::initData() {
  this->ip = this->getSocket().remote_endpoint().address().to_string();
  this->logger = Logger::create();
  this->parser = Parser::create();
  this->isActive = true;
  logger->logEvent(this->ip, "Client has connected\n");
  this->send("Bonjour\n");
}

void                          SocketIO::manageEventResponse() {
  switch (parser->responseType) {
    case Parser::CONNECT:
      this->hostname = parser->getResponse();
      break;
    case Parser::MESSAGE:
      logger->logMessage(this->ip, parser->getResponse());
      plugin->onReceiveMiddleware(this->hostname, this->ip, parser->getResponse());
      parser->responseType = Parser::NO_RES;
      break;
    case Parser::MOUSE:
      logger->logMessage(this->ip, parser->getResponse());
      plugin->onReceiveMiddleware(this->hostname, this->ip, parser->getResponse());
      parser->makeMessageResponse();
      this->manageEventResponse();
      break;
    case Parser::STATUS:
      logger->logMessage(this->ip, parser->getResponse());
      parser->responseType = Parser::NO_RES;
    default:
      break;
  }
  this->send("OK\r\n");
}

void                          SocketIO::applyReceiveMiddleware() {
  std::stringstream           ss(std::string(this->array.begin(), this->array.end()));
  std::string                 line;

  while (std::getline(ss, line)) {
    parser->receivePropsFromSocket(line);
    parser->manageEvent();
    this->manageEventResponse();
  }
  this->array.assign(0);
  this->receive();
}

void                          SocketIO::sendCallback(const boost::system::error_code& error, const std::string& data) {
  if (!error) {
    plugin->onSendMiddleware(this->hostname, this->ip, data);
  } else {
    this->isActive = false;
    logger->logError(this->ip, error.message());
  }
}

void                          SocketIO::receiveCallback(const boost::system::error_code& error) {
  if (!error) {
    this->applyReceiveMiddleware();
  } else {
    this->isActive = false;
    logger->logError(this->ip, error.message());
  }
}

void                          SocketIO::receive() {
  boost::asio::async_read(this->socket, boost::asio::buffer(this->array), boost::asio::transfer_at_least(1), boost::bind(&SocketIO::receiveCallback, shared_from_this(), boost::asio::placeholders::error));
}

void                          SocketIO::send(const std::string& data) {
  boost::asio::async_write(this->socket, boost::asio::buffer(data.c_str(), data.length()), boost::bind(&SocketIO::sendCallback, shared_from_this(), boost::asio::placeholders::error, data));
}
