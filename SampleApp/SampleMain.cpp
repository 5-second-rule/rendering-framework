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

	/*Vertex cube[8] = {
		{ { -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
		{ { 1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
		{ { 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }
	};

	UINT index[36] = {
		3, 1, 0,
		2, 1, 3,

		0, 5, 4,
		1, 5, 0,

		3, 4, 7,
		0, 4, 3,

		1, 6, 5,
		2, 6, 1,

		2, 7, 6,
		3, 7, 2,

		6, 4, 5,
		7, 4, 6,
	};*/

	/*
	// FBX Load
	FBXLoader fileLoaded;

	VertexBuffer* vbuf;
	IndexBuffer* ibuf;
	char* filePath = "../SampleApp/whitey.fbx";

	fileLoaded.loadFBXFile(filePath, &vbuf, &ibuf, renderer);
	*/

	VertexBuffer* vbuf;// = renderer->createVertexBuffer(cube, 8);
	IndexBuffer* ibuf;// = renderer->createIndexBuffer(index, 36);
	//Model* model = renderer->createModel(vbuf, ibuf);

	//model->move(Vector4(0, 0, 10));

	char* whiteBloodFbxFilePath = "../SampleApp/whitey.fbx";
	char* ecoliFbxFilePath = "../SampleApp/ecoli4_animated_binary.fbx";
	char* ecoliObjFilePath = "../SampleApp/Ecoli4_Object.obj";

	char* textureLocation = "../SampleApp/ecoli_TXTR.dds";
	char* textureLocationW = "../SampleApp/Wood.dds";

	Texture* texture = renderer->createTextureFromFile(textureLocation);

	Model* model = renderer->createModelFromFile(ecoliObjFilePath, &vbuf, &ibuf, texture);

	while (messagePump(window)) {
		renderer->clearFrame();

		model->draw();

		renderer->drawFrame();
	}

	delete model;
	delete vbuf;
	delete ibuf;
	delete texture;

	delete renderer;
	delete window;
}

