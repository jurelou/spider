#ifndef                                       SERVER_HPP_
# define                                      SERVER_HPP_

#include                                      <boost/asio/ssl.hpp>
#include                                      <boost/asio.hpp>
#include                                      <boost/bind.hpp>
#include                                      <boost/thread.hpp>
#include                                      "SocketIO.hpp"
#include                                      "Plugins.hpp"
#include                                      "Cmd.hpp"

class Server {
  public:
    Server(boost::asio::io_service& io_service, int port);
    ~Server();
  private:
    void                                      handleAccept();
    void                                      routine();
    void                                      routineCallback(const boost::system::error_code& error, std::size_t length);
    std::string                               getPasswordCallback();
    void                                      handleAcceptCallback(boost::shared_ptr<SocketIO> newClient, const boost::system::error_code& error);
    boost::shared_ptr<Plugins>                plug;
    boost::asio::ip::tcp::acceptor            serverAcceptor;
    boost::asio::ssl::context                 sslContext;
    std::vector<boost::shared_ptr<SocketIO> > clientPool;
    boost::asio::streambuf                    input;
    boost::asio::posix::stream_descriptor     inputHandler;
    std::vector<std::string>                  parse;
    boost::shared_ptr<Cmd>                    cmd;
};

#endif                                        /* !SERVER_HPP_ */
