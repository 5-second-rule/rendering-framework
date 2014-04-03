#include "Win32Window.h"


Win32Window::Win32Window(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow)
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

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 800, 600 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// this is all on one line becuse VC++ freaks out otherwise
	this->hWnd = CreateWindowEx(NULL, L"WindowClass1", L"Window Name", WS_OVERLAPPEDWINDOW, 300, 300, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL);

	ShowWindow(this->hWnd, nCmdShow);

}


Win32Window::~Win32Window()
{
}

void* Win32Window::getHandle() {
	return this->hWnd;
}

Window::MessageType Win32Window::getMessage() {
	MSG msg = { 0 };

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT || msg.message == WM_DESTROY || msg.message == WM_CLOSE) return Quit;
		else return Message;
	} else return None;
}
