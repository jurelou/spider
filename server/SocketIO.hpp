#ifndef                                 SOCKETIO_HPP_
# define                                SOCKETIO_HPP_

#include                                <boost/asio/ssl.hpp>
#include                                <boost/asio.hpp>
#include                                <boost/bind.hpp>
#include                                <boost/array.hpp>
#include                                <boost/enable_shared_from_this.hpp>
#include                                "INetwork.hpp"
#include                                "Logger.hpp"
#include                                "Parser.hpp"
#include                                "Plugins.hpp"

class SocketIO : public INetwork, public boost::enable_shared_from_this<SocketIO> {
  public:
    SocketIO(boost::asio::io_service& io_service, boost::asio::ssl::context& context, boost::shared_ptr<Plugins> plug);
    ~SocketIO();
    void                                receive();
    void                                initData();
    const std::string&                  getIp();
    const std::string&                  getHostname();
    void                                sendCallback(const boost::system::error_code& error, const std::string& data);
    void                                send(const std::string& data);
    void                                sslHandshake();
    void                                manageEventResponse();
    void                                sslHandshakeCallback(const boost::system::error_code& error);
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::lowest_layer_type&       getSocket();
    void                                applyReceiveMiddleware();
    void                                receiveCallback(const boost::system::error_code& error);
    static boost::shared_ptr<SocketIO>  create(boost::asio::io_service& io_service, boost::asio::ssl::context& context, boost::shared_ptr<Plugins> plug);
    bool                                isActive;
  private:
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>        socket;
    boost::array<char, 2048>            array;
    boost::shared_ptr<Logger>           logger;
    boost::shared_ptr<Parser>           parser;
    boost::shared_ptr<Plugins>          plugin;
    std::string                         ip;
    std::string                         hostname;
};

#endif                                  /* !SOCKETIO_HPP_ */
