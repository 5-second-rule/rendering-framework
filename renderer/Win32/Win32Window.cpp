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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = L"WindowClass1";
	wc.cbWndExtra = sizeof(Win32Window*);

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, screenWidth, screenHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// this is all on one line becuse VC++ freaks out otherwise
	this->hWnd = CreateWindowEx(NULL, L"WindowClass1", L"Window Name", WS_OVERLAPPEDWINDOW, 300, 300, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL);

	ShowWindow(this->hWnd, SW_SHOWNORMAL);

	this->oldWndproc = (WNDPROC)SetWindowLongPtr(this->hWnd, GWLP_WNDPROC, (LONG_PTR)SubclassWndProc);

	// store pointer back to c++ class
	SetWindowLongPtr(this->hWnd, 0, reinterpret_cast<LONG_PTR>(this));

	Win32Window::inp = Input();
}

LRESULT CALLBACK Win32Window::SubclassWndProc(
	HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
{
	Win32Window* cppWnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, 0));
	switch (wm) {
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			cppWnd->inp.forward = true;
			break;
		case 0x57: // W
			cppWnd->inp.up = true;
			break;
		case 0x41: // A
			cppWnd->inp.left = true;
			break;
		case 0x53: // S
			cppWnd->inp.down = true;
			break;
		case 0x44: // D
			cppWnd->inp.right = true;
			break;
		case 0x51: // Q
			cppWnd->inp.rollLeft = true;
			break;
		case 0x45: // E
			cppWnd->inp.rollRight = true;
			break;
		}
		return CallWindowProc(cppWnd->oldWndproc, hwnd, wm, wParam, lParam);
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_SPACE:
			cppWnd->inp.forward = false;
			break;
		case 0x57: // W
			cppWnd->inp.up = false;
			break;
		case 0x41: // A
			cppWnd->inp.left = false;
			break;
		case 0x53: // S
			cppWnd->inp.down = false;
			break;
		case 0x44: // D
			cppWnd->inp.right = false;
			break;
		case 0x51: // Q
			cppWnd->inp.rollLeft = false;
			break;
		case 0x45: // E
			cppWnd->inp.rollRight = false;
			break;
		}
		return CallWindowProc(cppWnd->oldWndproc, hwnd, wm, wParam, lParam);
    case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		return 0;
	default:
		return CallWindowProc(cppWnd->oldWndproc, hwnd, wm, wParam, lParam);
	}
}


Win32Window::~Win32Window()
{
}

void* Win32Window::getHandle() {
	return this->hWnd;
}

void* Win32Window::getInput() {
	return &(this->inp);
}

Window::MessageType Win32Window::getMessage() {
	MSG msg = { 0 };

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		UINT type = msg.message;
		DispatchMessage(&msg);

    if (type == WM_QUIT)
      return Quit;
		else 
      return Message;
	} 
  else 
    return None;
}

