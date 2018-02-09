#pragma once

#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost\asio\ssl.hpp>

class WindowsSslSocket
{
public:
	enum State
	{
		OFFLINE,
		AUTH,
		UNKNOWN,
		DISCONECTED,
		PAUSED,
		CONNECTED
	};

	WindowsSslSocket(boost::asio::io_service& io_service, boost::asio::ssl::context& context, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
	~WindowsSslSocket();

	void		connect();
	void		write(std::string msg);
	void		read();
	void		init();
	State		isOpen();
private:

	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
	boost::asio::ip::tcp::resolver::iterator _endpoint;


	bool verify_certificate(bool preverified, boost::asio::ssl::verify_context& ctx);
	void handle_connect(const boost::system::error_code& error);
	void handle_handshake(const boost::system::error_code& error);
	void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);



	boost::asio::streambuf writeBuffer;
	boost::asio::streambuf readBuffer;
	State		state;
};

