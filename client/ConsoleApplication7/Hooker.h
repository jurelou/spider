#pragma once

# define WIN32_LEAN_AND_MEAN
# define MOUSE_PRECISION 300

# include "Sender.h"
# include <Windows.h>
# include <iostream>
# include <math.h>

class Hooker {
public:

	static Hooker& getInstance();
	void runHookLoop();
	void clear();
	void receiveMouseCallback(int nCode, WPARAM wParam, LPARAM lParam);
	void receiveKeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam);
	void	setSender(Sender *s);
protected:
	Hooker();
	~Hooker();

	HHOOK _kHook;
	HHOOK _mHook;
	POINT _coord;
	Sender *_sender;

};

LRESULT CALLBACK cb_keyboard(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK cb_mouse(int nCode, WPARAM wParam, LPARAM lParam);