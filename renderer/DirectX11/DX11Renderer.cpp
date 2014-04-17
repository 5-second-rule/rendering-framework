#include "DX11Renderer.h"

#include <iostream>
#include <fstream>

#include "DX11VertexBuffer.h"
#include "DX11IndexBuffer.h"
#include "DX11Model.h"

DX11Renderer::DX11Renderer(Window* window) : Renderer()
{

	// Create a descriptor for our swap chain
	DXGI_SWAP_CHAIN_DESC desc;
	D3D_FEATURE_LEVEL featureLevel;

	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferCount = 1;
	desc.BufferDesc.Width = Window::screenWidth;
	desc.BufferDesc.Height = Window::screenHeight;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = (HWND)(window->getHandle());
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = windowed; //we should have a variable in config, for now its in header
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the device, device context, and swap chain
	HRESULT result = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&desc,
		&swapchain,
		&device,
		NULL,
		&context
		);
	if (FAILED(result)) throw std::runtime_error("Device and Swap Chain creation failed");

	// Get the backbuffer
	ID3D11Texture2D* pBackBuffer;
	if (FAILED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &pBackBuffer)))
		throw std::runtime_error("Couldn't get BackBuffer");

	// get a render target view on the backbuffer
	result = device->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);

	if (FAILED(result)) throw std::runtime_error("Couldn't create render target view on backbuffer");

	pBackBuffer->Release();
	pBackBuffer = NULL;

	// set the render target
	context->OMSetRenderTargets(1, &backbuffer, NULL);

	// setup constant buffer
	D3D11_BUFFER_DESC cb;
	ZeroMemory(&cb, sizeof(cb));
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.ByteWidth = sizeof(XMMATRIX) * 3;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;
	if (FAILED(device->CreateBuffer(&cb, NULL, &this->constantBuffer)))
		throw std::runtime_error("Couldn't create constant buffer");

	// describe the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof D3D11_VIEWPORT);

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	// added these in case we need them in future iterations
	//viewport.MinDepth = 0.0f;
	//viewport.MaxDepth = 1.0f;
	viewport.Width = (float)Window::screenWidth;
	viewport.Height = (float)Window::screenHeight;

	context->RSSetViewports(1, &viewport);

	// Camera and Perspective Matrices
	this->camera = new Camera(POINT(3, 5, -10), POINT(0, 0, 0), VECTOR(0, 1, 0),
		XM_PI / 4.0f, (float) Window::screenWidth / (float) Window::screenHeight, 1, 1000);

	/* ---------- */

	// fixed shaders for now
	this->vertexShader = new DX11VertexShader("vertex.cso", this->device);
	this->pixelShader = new DX11PixelShader("pixel.cso", this->device);

	// Input Layout for vertex buffers
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DX11Shader::Buffer VSbytecode = vertexShader->getBytecode();
	if (FAILED(result = device->CreateInputLayout(ied, 2, VSbytecode.buf, VSbytecode.len, &this->layout)))
		throw std::runtime_error("Could not create input layout");
	context->IASetInputLayout(this->layout);

	// set shaders
	context->VSSetShader(vertexShader->getVertexShader(), NULL, 0);
	context->PSSetShader(pixelShader->getPixelShader(), NULL, 0);
}


DX11Renderer::~DX11Renderer()
{
	delete camera;

	swapchain->SetFullscreenState(false, NULL);

	delete vertexShader;
	delete pixelShader;

	swapchain->Release();
	backbuffer->Release();
	constantBuffer->Release();
	device->Release();
	context->Release();
}

void DX11Renderer::clearFrame() {
	float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	context->ClearRenderTargetView(backbuffer, color);

	// set default stuff
	XMMATRIX matrices[3] = {
		DirectX::XMMatrixIdentity(),
		DirectX::XMMatrixTranspose(this->camera->getCameraInverse()),
		DirectX::XMMatrixTranspose(this->camera->getPerspective())
	};
	context->UpdateSubresource(constantBuffer, 0, NULL, &matrices, 0, 0);
	context->VSSetConstantBuffers(0, 1, &constantBuffer);

	// set shaders
	context->VSSetShader(vertexShader->getVertexShader(), NULL, 0);
	context->PSSetShader(pixelShader->getPixelShader(), NULL, 0);
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

Model* DX11Renderer::createModel(VertexBuffer* v, IndexBuffer* i) {
	return new DX11Model(v, i, context, this);
}

Camera* DX11Renderer::getCamera() {
	return this->camera;
}

void DX11Renderer::setObjectMatrix(ITransformable* t) {
	XMMATRIX matrices[3] = {
		DirectX::XMMatrixTranspose(t->getTransform()),
		DirectX::XMMatrixTranspose(this->camera->getCameraInverse()),
		DirectX::XMMatrixTranspose(this->camera->getPerspective())
	};
	context->UpdateSubresource(constantBuffer, 0, NULL, &matrices, 0, 0);
	context->VSSetConstantBuffers(0, 1, &constantBuffer);
}