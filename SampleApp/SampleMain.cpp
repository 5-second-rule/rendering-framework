#include <windows.h>

#include "renderer/Renderer.h"
#include "renderer/Window.h"

#include "renderer/FBXLoader.h"

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
		{ XMFLOAT3(-0.5, 0, 0), XMFLOAT4(1, 0, 0, 1) },
		{ XMFLOAT3(0, 0.5, 0), XMFLOAT4(0, 1, 0, 1) },
		{ XMFLOAT3(0.5, 0, 0), XMFLOAT4(0, 0, 1, 1) }
	};

	UINT index[3] = { 0, 1, 2 };

	FBXLoader fileLoaded;

	VertexBuffer* vbuf;
	IndexBuffer* ibuf;

	char* filePath = "C:\\Users\\Kyler\\Desktop\\whitey.fbx";

	fileLoaded.loadFBXFile(filePath, &vbuf, &ibuf, renderer);

	//VertexBuffer* vbuf = renderer->createVertexBuffer(triangle, 3);
	//IndexBuffer* ibuf = renderer->createIndexBuffer(index, 3);
	Model* model = renderer->createModel(vbuf, ibuf);

	//((DX11Model*)model)->loadFBXModel();

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



