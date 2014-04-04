#include <windows.h>

#include "Win32/Win32Window.h"
#include "DirectX11/DX11Renderer.h"
#include "BaseApp.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Win32Window window(hInstance, lpCmdLine, nCmdShow);
	DX11Renderer renderer(window);
	BaseApp app(window, renderer);
	app.start();
}