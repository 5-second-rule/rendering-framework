#pragma once

#include <Windows.h>

#include "render-framework.h"

#include "Window.h"

#include "Windowsx.h"

#include "Win32Input.h"

namespace Transmission {
	class Win32Window :
		public Window
	{
	protected:
		HINSTANCE hInstance;
		HWND hWnd;
		WNDPROC oldWndproc;
		Win32Input input;

		static LRESULT CALLBACK SubclassWndProc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam);
	public:
		Win32Window(HINSTANCE);
		~Win32Window();

		virtual void* getHandle();
		virtual const Input* getInput();
		virtual Window::MessageType getMessage();
	};

}