#include <Windows.h>

#include "Window.h"
#include "Renderer.h"

//TODO: Fix the way these are included
#include "DirectX11/DX11VertexBuffer.h"
#include "DirectX11/DX11IndexBuffer.h"
#include "DirectX11/DX11Model.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Transmission::Window* window = Transmission::Window::createWindow(hInstance, L"Sample App", 800, 600);
	Transmission::Renderer* renderer = Transmission::Renderer::createRenderer(window);

	Transmission::Vertex vertices [] = {
		{ {  1,  1, 0 }, { 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
		{ {  1, -1, 0 }, { 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
		{ { -1,  1, 0 }, { 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
		{ { -1, -1, 0 }, { 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
	};

	unsigned int indices [] = {
		2, 0, 3,
		0, 1, 3
	};

	Transmission::VertexBuffer* vertexBuffer = renderer->create<Transmission::VertexBuffer>(vertices, 4);
	Transmission::IndexBuffer* indexBuffer = renderer->create<Transmission::IndexBuffer>(indices, 6);

	//Transmission::Texture* texture = renderer->create<Transmission::Texture>("foo");

	//Transmission::Model* model = renderer->create<Transmission::Model>(vertexBuffer, indexBuffer, { texture });

}