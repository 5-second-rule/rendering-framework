#include "Win32Window.h"


Win32Window::Win32Window(HINSTANCE hInstance)
	: Window()
	, hInstance(hInstance)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof WNDCLASSEX);

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	// wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";
	wc.cbWndExtra = sizeof(Win32Window*);

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 800, 600 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// this is all on one line becuse VC++ freaks out otherwise
	this->hWnd = CreateWindowEx(NULL, L"WindowClass1", L"Window Name", WS_OVERLAPPEDWINDOW, 300, 300, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL);

	ShowWindow(this->hWnd, SW_SHOWNORMAL);


	this->oldWndproc = (WNDPROC)SetWindowLongPtr(this->hWnd, GWLP_WNDPROC, (LONG_PTR)SubclassWndProc);

	// store pointer back to c++ class
	SetWindowLongPtr(this->hWnd, 0, reinterpret_cast<LONG_PTR>(this));
}

LRESULT CALLBACK Win32Window::SubclassWndProc(
	HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
{
	Win32Window* cppWnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, 0));
	switch (wm) {
		case WM_CLOSE:
			PostMessage(hwnd, wm, wParam, lParam);
			return 0;
		default:
			break;
	}

	return CallWindowProc(cppWnd->oldWndproc, hwnd, wm, wParam, lParam);
}


Win32Window::~Win32Window()
{
}

void* Win32Window::getHandle() {
	return this->hWnd;
}

Window::MessageType Win32Window::getMessage() {
	MSG msg = { 0 };

	if (PeekMessage(&msg, this->hWnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		UINT type = msg.message;
		DispatchMessage(&msg);

		if (type == WM_QUIT || type == WM_DESTROY || type == WM_CLOSE) return Quit;
		else return Message;
	} else return None;
}
