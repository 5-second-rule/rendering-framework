#include <windows.h>

#include "renderer/Renderer.h"
#include "renderer/Window.h"

#include "renderer/FBXLoader.h"

#include "renderer/Input.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool messagePump(Transmission::Window* window) {
	Transmission::Window::MessageType t;
	while ((t = window->getMessage()) != Transmission::Window::MessageType::None) {
		if (t == Transmission::Window::MessageType::Quit) return false;
	}

	return true;
}

static int reduceSpam = 1;

void moveBlob(Transmission::Window* w, Transmission::Model* m) {
	float moveAmt = 1.0f / 1000.0f;
	Transmission::Input* inp = (Transmission::Input*) w->getInput();

	if (inp->keys[Transmission::Input::KEY_W] == Transmission::Input::STATE_DOWN) {
		m->move(Transmission::Vector4(0, moveAmt, 0));
	}
	if (inp->keys[Transmission::Input::KEY_A] == Transmission::Input::STATE_DOWN) {
		m->move(Transmission::Vector4(-moveAmt, 0, 0));
	}
	if (inp->keys[Transmission::Input::KEY_S] == Transmission::Input::STATE_DOWN) {
		m->move(Transmission::Vector4(0, -moveAmt, 0));
	}
	if (inp->keys[Transmission::Input::KEY_D] == Transmission::Input::STATE_DOWN) {
		m->move(Transmission::Vector4(moveAmt, 0, 0));
	}	
	if (inp->keys[Transmission::Input::KEY_SP] == Transmission::Input::STATE_DOWN) {
		m->move(Transmission::Vector4(0, 0, moveAmt));
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Transmission::Window* window = Transmission::Window::createWindow(hInstance);
	Transmission::Renderer* renderer = Transmission::Renderer::createRenderer(window);

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

	Transmission::VertexBuffer* vbuf;// = renderer->createVertexBuffer(cube, 8);
	Transmission::IndexBuffer* ibuf;// = renderer->createIndexBuffer(index, 36);
	//Model* model = renderer->createModel(vbuf, ibuf);

	//model->move(Vector4(0, 0, 10));

	char* whiteBloodFbxFilePath = "../SampleApp/whitey.fbx";
	char* ecoliFbxFilePath = "../SampleApp/ecoli6_nomedia.fbx";
	char* ecoliObjFilePath = "../SampleApp/Ecoli6_obj.obj";
	char* boxFbxFilePath = "../SampleApp/cube.fbx";

	char* textureLocation = "../SampleApp/ecoli6_TXTR.dds";
	char* textureLocationW = "../SampleApp/Wood.dds";
	char* whiteTexture = "../SampleApp/whitebloodcell_3_TXTR.dds";
	char* cubeTexture = "../SampleApp/cube_uvmap2.dds";

	Transmission::Texture* texture = renderer->createTextureFromFile(textureLocationW);

	Transmission::Model* model = renderer->createModelFromFile(ecoliFbxFilePath, &vbuf, &ibuf, texture);

	if (model == NULL) throw std::runtime_error("YO! DA MODEL VAS NULL!");

	while (messagePump(window)) {
		renderer->clearFrame();

		model->rotate(Transmission::Vector(0.00f, 0.01f, 0.0f));
		model->draw();

		renderer->drawFrame();

		moveBlob(window, model); // temp input handler
	}

	delete model;
	delete vbuf;
	delete ibuf;
	delete texture;

	delete renderer;
	delete window;
}

