#include "Win32Window.h"

#include "Renderer.h"
namespace Transmission {

	Win32Window::Win32Window(HINSTANCE hInstance, const wchar_t* name, unsigned int width, unsigned int height)
		: Window(name, width, height)
		, hInstance(hInstance)
	{
		WNDCLASSEX wc;

		ZeroMemory(&wc, sizeof WNDCLASSEX);

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = DefWindowProc;
		wc.hInstance = hInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszClassName = L"WindowClass1";
		wc.cbWndExtra = sizeof(Win32Window*);

		RegisterClassEx(&wc);

		RECT wr = { 0, 0, width, height };
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

		// this is all on one line becuse VC++ freaks out otherwise
		this->hWnd = CreateWindowEx(NULL, L"WindowClass1", name, WS_OVERLAPPEDWINDOW, 300, 300, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL);

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
		Input::Key key;

		switch (wm) {
		case WM_KEYDOWN:
			key = Input::findKey(wParam);
			if (key != Input::NOT_FOUND)
				cppWnd->inp.keyDown(key);
			break;
		case WM_KEYUP:
			key = Input::findKey(wParam);
			if (key != Input::NOT_FOUND)
				cppWnd->inp.keyUp(key);
			break;

			// Need to handle mouse buttons directly
		case WM_LBUTTONDOWN:
			cppWnd->inp.keyDown(Input::KEY_LMB);
			break;
		case WM_RBUTTONDOWN:
			cppWnd->inp.keyDown(Input::KEY_RMB);
			break;
		case WM_LBUTTONUP:
			cppWnd->inp.keyUp(Input::KEY_LMB);
			break;
		case WM_RBUTTONUP:
			cppWnd->inp.keyUp(Input::KEY_RMB);
			break;
		case WM_DESTROY:
			PostQuitMessage(EXIT_SUCCESS);
			return 0;

		case WM_SIZE:
			if (!(wParam == SIZE_MAXIMIZED)) break;
		case WM_EXITSIZEMOVE:
				if (cppWnd->renderer != NULL) {
					RECT r;
					GetWindowRect(hwnd, &r);
					cppWnd->renderer->resize(r.right - r.left, r.bottom - r.top);
				}
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
		return &(this->inp);
	}

	Window::MessageType Win32Window::getMessage() {
		MSG msg = { 0 };

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			UINT type = msg.message;
			DispatchMessage(&msg);
			
			if (type == WM_SIZE) {
				throw 1;
				if (this->renderer != NULL) {
					RECT r;
					GetWindowRect(this->hWnd, &r);
					this->renderer->resize(r.right - r.left, r.bottom - r.top);
				}
			}

			if (type == WM_QUIT)
				return Quit;
			else
				return Message;
		}
		else
			return None;
	}

}