#include <Windows.h>

#include "Window.h"
#include "Renderer.h"

//TODO: Fix the way these are included
#include "DirectX11/DX11VertexBuffer.h"
#include "DirectX11/DX11IndexBuffer.h"
#include "DirectX11/DX11Model.h"
#include "DirectX11/DX11Texture.h"
#include "DirectX11/DX11VertexShader.h"
#include "DirectX11/DX11PixelShader.h"

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

	auto* vertexBuffer = renderer->create<Transmission::VertexBuffer>(vertices, 4);
	auto* indexBuffer = renderer->create<Transmission::IndexBuffer>(indices, 6);

	auto* texture = renderer->create<Transmission::Texture>("Wood.dds");

	auto* model = renderer->create<Transmission::Model>(vertexBuffer, indexBuffer, std::initializer_list<Transmission::Texture*>{ texture });

	auto* pixelShader = renderer->create<Transmission::PixelShader>("defaultPixel.cso");
	auto* vertexShader = renderer->create<Transmission::VertexShader>("defaultVertex.cso");

	while (true) {
		renderer->clearFrame();
		model->draw(*vertexShader, *pixelShader);
		renderer->drawFrame();

		while (Transmission::Window::MessageType msg = window->getMessage()) {
			if (msg == Transmission::Window::MessageType::Quit) return 0;
		}
	}

}