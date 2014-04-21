#include "Win32Window.h"
#include "Input.h"
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
		case 0x57: // W
			cppWnd->inp.keyDown(Input::KEY_W);
			break;
		case 0x41: // A
			cppWnd->inp.keyDown(Input::KEY_A);
			break;
		case 0x53: // S
			cppWnd->inp.keyDown(Input::KEY_S);
			break;
		case 0x44: // D
			cppWnd->inp.keyDown(Input::KEY_D);
			break;
		case 0x51: // Q
			cppWnd->inp.keyDown(Input::KEY_Q);
			break;
		case 0x45: // E
			cppWnd->inp.keyDown(Input::KEY_E);
			break;
		case 0x5A: // Z
			cppWnd->inp.keyDown(Input::KEY_Z);
			break;
		case 0x58: // X
			cppWnd->inp.keyDown(Input::KEY_X);
			break;
		case 0x43: // C
			cppWnd->inp.keyDown(Input::KEY_C);
			break;
		case 0x56: // V
			cppWnd->inp.keyDown(Input::KEY_V);
			break;
		case 0x31: // 1
			cppWnd->inp.keyDown(Input::KEY_1);
			break;
		case 0x32: // 2
			cppWnd->inp.keyDown(Input::KEY_2);
			break;
		case 0x33: // 3
			cppWnd->inp.keyDown(Input::KEY_3);
			break;
		case 0x34: // 4
			cppWnd->inp.keyDown(Input::KEY_4);
			break;
		case 0x35: // 5
			cppWnd->inp.keyDown(Input::KEY_5);
			break;
		case VK_SPACE:
			cppWnd->inp.keyDown(Input::KEY_SP);
			break;
		case VK_CONTROL:
			cppWnd->inp.keyDown(Input::KEY_CTRL);
			break;
		case VK_SHIFT:
			cppWnd->inp.keyDown(Input::KEY_SHFT);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		cppWnd->inp.keyDown(Input::KEY_LMB);
		break;
	case WM_RBUTTONDOWN:
		cppWnd->inp.keyDown(Input::KEY_RMB);
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case 0x57: // W
			cppWnd->inp.keyUp(Input::KEY_W);
			break;
		case 0x41: // A
			cppWnd->inp.keyUp(Input::KEY_A);
			break;
		case 0x53: // S
			cppWnd->inp.keyUp(Input::KEY_S);
			break;
		case 0x44: // D
			cppWnd->inp.keyUp(Input::KEY_D);
			break;
		case 0x51: // Q
			cppWnd->inp.keyUp(Input::KEY_Q);
			break;
		case 0x45: // E
			cppWnd->inp.keyUp(Input::KEY_E);
			break;
		case 0x5A: // Z
			cppWnd->inp.keyUp(Input::KEY_Z);
			break;
		case 0x58: // X
			cppWnd->inp.keyUp(Input::KEY_X);
			break;
		case 0x43: // C
			cppWnd->inp.keyUp(Input::KEY_C);
			break;
		case 0x56: // V
			cppWnd->inp.keyUp(Input::KEY_V);
			break;
		case 0x31: // 1
			cppWnd->inp.keyUp(Input::KEY_1);
			break;
		case 0x32: // 2
			cppWnd->inp.keyUp(Input::KEY_2);
			break;
		case 0x33: // 3
			cppWnd->inp.keyUp(Input::KEY_3);
			break;
		case 0x34: // 4
			cppWnd->inp.keyUp(Input::KEY_4);
			break;
		case 0x35: // 5
			cppWnd->inp.keyUp(Input::KEY_5);
			break;
		case VK_SPACE:
			cppWnd->inp.keyUp(Input::KEY_SP);
			break;
		case VK_CONTROL:
			cppWnd->inp.keyUp(Input::KEY_CTRL);
			break;
		case VK_SHIFT:
			cppWnd->inp.keyUp(Input::KEY_SHFT);
			break;
		case VK_LBUTTON:
			cppWnd->inp.keyUp(Input::KEY_LMB);
			break;
		case VK_RBUTTON:
			cppWnd->inp.keyUp(Input::KEY_RMB);
			break;
		}
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

