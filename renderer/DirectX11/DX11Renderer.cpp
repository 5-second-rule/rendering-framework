#include "DX11Renderer.h"

#include <iostream>
#include <fstream>

#include "DX11VertexBuffer.h"
#include "DX11IndexBuffer.h"

DX11Renderer::DX11Renderer(Window& window) : Renderer()
{
	/* TODO: Windows window stuff */

	DXGI_SWAP_CHAIN_DESC desc;

	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = (HWND)(window.getHandle());
	desc.SampleDesc.Count = 4;
	desc.Windowed = true;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&desc,
		&swapchain,
		&device,
		NULL,
		&context
	);

	ID3D11Texture2D* pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &pBackBuffer);

	device->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	context->OMSetRenderTargets(1, &backbuffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof D3D11_VIEWPORT);

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	context->RSSetViewports(1, &viewport);

	// fixed shaders for now
	this->vertexShader = new DX11VertexShader("vertex.cso", this->device);
	this->pixelShader = new DX11PixelShader("pixel.cso", this->device);

	context->VSSetShader(vertexShader->getVertexShader(), NULL, 0);
	context->PSSetShader(pixelShader->getPixelShader(), NULL, 0);

	// Input Layout for vertex buffers
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DX11Shader::Buffer VSbytecode = vertexShader->getBytecode();
	device->CreateInputLayout(ied, 2, VSbytecode.buf, VSbytecode.len, &this->layout);
	context->IASetInputLayout(this->layout);
}


DX11Renderer::~DX11Renderer()
{
	swapchain->SetFullscreenState(false, NULL);

	delete vertexShader;
	delete pixelShader;

	swapchain->Release();
	backbuffer->Release();
	device->Release();
	context->Release();
}

void DX11Renderer::clearFrame() {
	float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	context->ClearRenderTargetView(backbuffer, color);
}

void DX11Renderer::drawFrame() {
	swapchain->Present(0, 0);
}

VertexBuffer* DX11Renderer::createVertexBuffer(Vertex vertices[], size_t num) {
	return new DX11VertexBuffer(vertices, num, this->device, this->context);
}

IndexBuffer* DX11Renderer::createIndexBuffer(unsigned int indices[], size_t num) {
	return new DX11IndexBuffer(indices, num, this->device, this->context);
}
