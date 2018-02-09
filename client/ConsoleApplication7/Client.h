#pragma once

#include "WindowsSocket.h"
#include <iostream>
#include <string>
#include <thread>
#include <queue>
#include <fstream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost\asio\ssl.hpp>
class Client
{
public:
	Client(boost::asio::io_service& io_service, boost::asio::ssl::context& context, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
	~Client();

	
	void				init();
	WindowsSslSocket	*getSocket() const;
	WindowsSslSocket::State				getState();
	void				restart();
private:
	boost::asio::io_service							&_io_service;
	boost::asio::ip::tcp::resolver::iterator		_endpoint;
	boost::asio::ssl::context						&_ctx;

	WindowsSslSocket	*socket;
};

