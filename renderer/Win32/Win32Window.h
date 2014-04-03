#pragma once

#include <Windows.h>

#include "Window.h"

class Win32Window :
	public Window
{
protected:
	HINSTANCE hInstance;
	HWND hWnd;
public:
	Win32Window(HINSTANCE, LPSTR, int);
	~Win32Window();

	virtual void* getHandle();
	virtual Window::MessageType getMessage();
};

