#include "DX11Renderer.h"


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
	swapchain->SetFullscreenState(false, NULL);
	swapchain->Release();
	backbuffer->Release();
	device->Release();
	context->Release();
}

void DX11Renderer::initializePipeline() {
	
	//ID3D10Blob *VS, *PS;
	//D3DX11CompileFromFile(L"vertex.hlsl", 0, 0, "VShader", "vs_5_0", 0, 0, 0, &VS, 0, 0);
	//D3DX11CompileFromFile(L"pixel.hlsl", 0, 0, "PSHader", "ps_5_0", 0, 0, 0, &VS, 0, 0);
}

void DX11Renderer::renderFrame() {
	float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	context->ClearRenderTargetView(backbuffer, color);

	// rendering happens here

	swapchain->Present(0, 0);
}
