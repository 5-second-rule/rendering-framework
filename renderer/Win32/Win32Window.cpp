#include "Win32Window.h"
#include "Windowsx.h"
namespace Transmission {

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
		wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
		wc.lpszClassName = L"WindowClass1";
		wc.cbWndExtra = sizeof(Win32Window*);

		RegisterClassEx(&wc);

		RECT wr = { 0, 0, screenWidth, screenHeight };
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

		// this is all on one line becuse VC++ freaks out otherwise
		this->hWnd = CreateWindowEx(NULL, L"WindowClass1", L"Window Name", WS_OVERLAPPEDWINDOW, 300, 300, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL);

		ShowWindow(this->hWnd, SW_SHOWNORMAL);

		this->oldWndproc = (WNDPROC) SetWindowLongPtr(this->hWnd, GWLP_WNDPROC, (LONG_PTR) SubclassWndProc);

		// store pointer back to c++ class
		SetWindowLongPtr(this->hWnd, 0, reinterpret_cast<LONG_PTR>(this));

		Win32Window::input = Win32Input();
	}

	LRESULT CALLBACK Win32Window::SubclassWndProc(
		HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
	{
		Win32Window* cppWnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, 0));
		Win32Input* input = (Win32Input*) cppWnd->getInput();

		switch (wm) {
		case WM_DESTROY:
			PostQuitMessage(EXIT_SUCCESS);
			return 0;

		case WM_KEYDOWN:
			if (input != NULL)
				input->keyDown(wParam);
			break;
		case WM_KEYUP:
			if (input != NULL)
				input->keyUp(wParam);
			break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_XBUTTONDOWN:
			if (input != NULL)
				input->mouseDown(wParam);
			break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_XBUTTONUP:
			if (input != NULL)
				input->mouseUp(wParam);
			break;

		case WM_MOUSEMOVE:
			if (input != NULL)
				input->setMousePosition(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

		default:
			return CallWindowProc(cppWnd->oldWndproc, hwnd, wm, wParam, lParam);
		}

		return CallWindowProc(cppWnd->oldWndproc, hwnd, wm, wParam, lParam);
	}


	Win32Window::~Win32Window()
	{
	}

	void* Win32Window::getHandle() {
		return this->hWnd;
	}

	const Input* Win32Window::getInput() {
		return &(this->input);
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

}