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

	VertexBuffer* vbuf;
	IndexBuffer* ibuf;

	char* whiteBloodFbxFilePath = "../SampleApp/whitey.fbx";
	char* ecoliFbxFilePath = "../SampleApp/ecoli4_animated_binary.fbx";
	char* ecoliObjFilePath = "../SampleApp/Ecoli4_Object.obj";

	Model* model = renderer->createModelFromFile(ecoliFbxFilePath, &vbuf, &ibuf);

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



