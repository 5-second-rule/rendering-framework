#include "DX11Renderer.h"

#include <iostream>
#include <fstream>

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
}


DX11Renderer::~DX11Renderer()
{
	pixelShader->Release();
	vertexShader->Release();
	swapchain->SetFullscreenState(false, NULL);
	swapchain->Release();
	backbuffer->Release();
	device->Release();
	context->Release();
}

void DX11Renderer::initializePipeline() {
	
	// Vertex Shader
	std::ifstream vs("vertex.cso", std::ifstream::binary);

	vs.seekg(0, vs.end);
	int vs_length = vs.tellg();
	vs.seekg(0, vs.beg);

	char* vs_buf = new char[vs_length];

	vs.read(vs_buf, vs_length);
	vs.close();

	// Add the shader to things
	this->device->CreateVertexShader(vs_buf, vs_length, NULL, &vertexShader);

	delete[] vs_buf;
	
	// Pixel Shader
	std::ifstream ps("pixel.cso", std::ifstream::binary);

	ps.seekg(0, ps.end);
	int ps_length = ps.tellg();
	ps.seekg(0, ps.beg);

	char* ps_buf = new char[ps_length];

	ps.read(ps_buf, ps_length);
	ps.close();

	this->device->CreatePixelShader(ps_buf, ps_length, NULL, &pixelShader);

	delete[] ps_buf;

}

void DX11Renderer::renderFrame() {
	float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	context->ClearRenderTargetView(backbuffer, color);

	// rendering happens here

	swapchain->Present(0, 0);
}
