#include "Hooker.h"

extern HHOOK g_hook[2];

Hooker::Hooker() {
	_coord.x = 0;
	_coord.y = 0;
	_kHook = SetWindowsHookEx(WH_KEYBOARD_LL, cb_keyboard, NULL, 0);
	if (!_kHook) {
		throw GetLastError();
	}
	_mHook = SetWindowsHookEx(WH_MOUSE_LL, cb_mouse, NULL, 0);
	if (!_mHook) {
		throw GetLastError();
	}
}

Hooker::~Hooker() {
	clear();
}

Hooker& Hooker::getInstance() {
	static Hooker	instance;
	return instance;
}

void Hooker::runHookLoop()
{
	MSG message;
	ZeroMemory(&message, sizeof(message));
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void Hooker::clear() {
	UnhookWindowsHookEx(_kHook);
	UnhookWindowsHookEx(_mHook);
}

void Hooker::receiveKeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	_sender->sendKey(nCode, wParam, lParam);
}
void Hooker::receiveMouseCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	MSLLHOOKSTRUCT* mouseStruct = (MSLLHOOKSTRUCT*)lParam;
	if (wParam == WM_MOUSEMOVE) {
		if (((std::abs(std::abs(mouseStruct->pt.x) - std::abs(_coord.x)) >= MOUSE_PRECISION) ||
			std::abs(std::abs(mouseStruct->pt.y) - std::abs(_coord.y)) >= MOUSE_PRECISION)) {
			_coord.x = mouseStruct->pt.x;
			_coord.y = mouseStruct->pt.y;
			_sender->sendMouse(nCode, wParam, lParam, mouseStruct);
		}
	}
	else {
		_sender->sendClick(nCode, wParam, lParam);
	}
}
void	Hooker::setSender(Sender *s)
{
	_sender = s;
}

LRESULT CALLBACK cb_keyboard(int nCode, WPARAM wParam, LPARAM lParam) {
	Hooker& hooker = Hooker::getInstance();
	if (nCode >= 0)
		hooker.receiveKeyboardCallback(nCode, wParam, lParam);
	return CallNextHookEx(0, nCode, wParam, lParam);
}

LRESULT CALLBACK cb_mouse(int nCode, WPARAM wParam, LPARAM lParam) {
	Hooker& hooker = Hooker::getInstance();
	if (nCode >= 0)
		hooker.receiveMouseCallback(nCode, wParam, lParam);
	return CallNextHookEx(0, nCode, wParam, lParam);
}
