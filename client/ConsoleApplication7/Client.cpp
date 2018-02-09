#include "Client.h"

Client::Client(boost::asio::io_service& io_service, boost::asio::ssl::context& context, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	: _ctx(context),
	_io_service(io_service),
	_endpoint(endpoint_iterator)
{
	socket = new WindowsSslSocket(_io_service, _ctx, _endpoint);
}

Client::~Client()
{}

void	Client::init()
{
	socket->init();
}

WindowsSslSocket	*Client::getSocket() const
{
	return (socket);
}
void			Client::restart()
{
	std::cout << "restarting" << std::endl;
	delete this->socket;
	socket = new WindowsSslSocket(_io_service, _ctx, _endpoint);
	this->init();
}

WindowsSslSocket::State			Client::getState()
{
	return (this->socket->isOpen());
}

