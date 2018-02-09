#include "WindowsSocket.h"


WindowsSslSocket::WindowsSslSocket(boost::asio::io_service& io_service, boost::asio::ssl::context& context, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
:	socket_(io_service, context),
	_endpoint(endpoint_iterator),
	state(OFFLINE)
{
}

void	WindowsSslSocket::init()
{
	readBuffer.prepare(1024);
	writeBuffer.prepare(1024);
	socket_.set_verify_mode(boost::asio::ssl::verify_peer);
	socket_.set_verify_callback(
		boost::bind(&WindowsSslSocket::verify_certificate, this, _1, _2));
	this->connect();
}

bool WindowsSslSocket::verify_certificate(bool preverified,
	boost::asio::ssl::verify_context& ctx)
{
	char subject_name[256];
	X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
	X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
	std::cout << "Verifying " << subject_name << "\n";

	return preverified;
}

WindowsSslSocket::~WindowsSslSocket()
{
}


void	WindowsSslSocket::connect()
{
	std::cout << "connecting" << std::endl;
	boost::asio::async_connect(socket_.lowest_layer(), _endpoint,
		boost::bind(&WindowsSslSocket::handle_connect, this,
			boost::asio::placeholders::error));
}

void WindowsSslSocket::handle_connect(const boost::system::error_code& error)
{
	if (!error)
	{
		state = CONNECTED;
		socket_.async_handshake(boost::asio::ssl::stream_base::client,
			boost::bind(&WindowsSslSocket::handle_handshake, this,
				boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Connect failed: " << error.message() << "\n";
		state = OFFLINE;
		this->connect();
	}
}
void WindowsSslSocket::handle_handshake(const boost::system::error_code& error)
{
	if (!error)
	{
		this->write("connect\n");
		this->read();
		state = AUTH;
	}
	else
	{
		state = OFFLINE;
		std::cout << "Handshake failed: " << error.message() << "\n";
	}
}

void	WindowsSslSocket::write(std::string msg)
{
	std::ostream os(&writeBuffer);
	os << msg;
	boost::asio::async_write(socket_,
		writeBuffer,
		boost::bind(&WindowsSslSocket::handle_write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void	WindowsSslSocket::read()
{
	boost::asio::async_read(socket_,
		this->readBuffer,
		boost::asio::transfer_at_least(1),
		boost::bind(&WindowsSslSocket::handle_read,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void WindowsSslSocket::handle_write(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{
		std::cout << "writed " << "|" << "|"<< std::endl;
		writeBuffer.consume(writeBuffer.size());
	}
	else
	{
		std::cout << "Write failed: " << error.message() << "\n";
		state = DISCONECTED;
	}
}

void WindowsSslSocket::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{
		std::cout << "reading ..." << std::endl;
		std::istream s(&readBuffer);
		std::string str;
		while (std::getline(s, str))
		{
			if (str == "PAUSE\r")
				state = PAUSED;
			if (str == "RESUME\r")
				state = AUTH;
			if (str == "STOP\r")
				exit(1);
			std::cout << str << std::endl;
		}
		this->read();
	}
	else
	{
		std::cout << "Read failed: " << error.message() << "\n";
		state = DISCONECTED;
	}
}

WindowsSslSocket::State		WindowsSslSocket::isOpen()
{
	return (state);
}