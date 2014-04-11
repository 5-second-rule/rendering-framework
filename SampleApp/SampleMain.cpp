#include <windows.h>

#include "renderer/Renderer.h"
#include "renderer/Window.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool messagePump(Window* window) {
	Window::MessageType t;
	while ((t = window->getMessage()) != Window::MessageType::None) {
		if (t == Window::MessageType::Quit) return false;
	}

	return true;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Window* window = Window::createWindow(hInstance);
	Renderer* renderer = Renderer::createRenderer(window);

	Vertex triangle[3] = {
		{ XMFLOAT3(0, 0, 1), XMFLOAT4(1, 0, 0, 1) },
		{ XMFLOAT3(0, 1, 1), XMFLOAT4(0, 1, 0, 1) },
		{ XMFLOAT3(1, 0, 1), XMFLOAT4(0, 0, 1, 1) }
	};

	UINT index[3] = { 0, 1, 2 };

	VertexBuffer* vbuf = renderer->createVertexBuffer(triangle, 3);
	IndexBuffer* ibuf = renderer->createIndexBuffer(index, 3);
	Model* model = renderer->createModel(vbuf, ibuf);

	while (messagePump(window)) {

		renderer->clearFrame();

		model->draw();

		renderer->drawFrame();
	}

	delete model;
	delete vbuf;
	delete ibuf;

	delete renderer;
	delete window;
}



