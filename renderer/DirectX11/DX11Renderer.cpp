#define _USE_MATH_DEFINES
#include "DX11Renderer.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <stdexcept>

#define _EXPORT_COMMON
#include "common/Matrix4.h"
#include "common/Vector4.h"
using namespace Common;

#include "DX11VertexBuffer.h"
#include "DX11IndexBuffer.h"
#include "DX11Model.h"
#include "DX11Texture.h"
#include "DX11VertexShader.h"
#include "DX11PixelShader.h"
#include "DX11Timer.h"

namespace Transmission {


	DX11Renderer::DX11Renderer(Window* window, char* vertex, char* pixel) : Renderer()
	{
		swapchain = NULL; device = NULL; context = NULL;
		this->setupDeviceAndSwapChain(window);

		backbuffer = NULL;
		this->setupBackBuffer();

		camera = NULL;
		this->setupViewportAndCamera(window);
		/* ---------- */

		renderTimer = NULL;
		perFrameBuffer = NULL; perVertexBuffer = NULL; timeBuffer = NULL;
		this->setupConstantBuffer();

		defaultVertexShader = NULL; defaultPixelShader = NULL; layout = NULL;
		ied = NULL;
		this->setupShaders(vertex, pixel);
	}


	DX11Renderer::~DX11Renderer()
	{
		delete camera;

		swapchain->SetFullscreenState(false, NULL);

		delete defaultVertexShader;
		delete defaultPixelShader;
		delete[] ied;
		delete renderTimer;

		defaultVertexShader = NULL;
		defaultPixelShader = NULL;
		ied = NULL;
		renderTimer = NULL;

		swapchain->Release();
		backbuffer->Release();
		perFrameBuffer->Release();
		perVertexBuffer->Release();
		timeBuffer->Release();
		device->Release();
		context->Release();
	}

	//=============================================//
	//          Constructor Setup Methods          //
	//=============================================//

	void DX11Renderer::setupDeviceAndSwapChain(Window* window) {
		if (device != NULL || swapchain != NULL || context != NULL) {
			throw std::runtime_error("You can only setup the device and swap chain once");
		}

		// Create a descriptor for our swap chain
		DXGI_SWAP_CHAIN_DESC desc;

		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

		desc.BufferCount = 1;
		desc.BufferDesc.Width = Window::screenWidth;
		desc.BufferDesc.Height = Window::screenHeight;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.OutputWindow = (HWND) (window->getHandle());
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Windowed = this->windowed; //we should have a variable in config, for now its in header
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3
		};

		UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// Create the device, device context, and swap chain
		HR(D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			createDeviceFlags,
			featureLevels,
			4,
			D3D11_SDK_VERSION,
			&desc,
			&this->swapchain,
			&this->device,
			NULL,
			&this->context
		));

	}

	void DX11Renderer::setupBackBuffer() {

		if (this->backbuffer != NULL) {
			throw std::runtime_error("You can only setup the BackBuffer once");
		}

		// Get the backbuffer
		ID3D11Texture2D* pBackBuffer;
		HR(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &pBackBuffer));

		// get a render target view on the backbuffer
		HR(device->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer));




		// -----
		// Depth Stencil
		// -----

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));

		depthStencilDesc.Width = Window::screenWidth;
		depthStencilDesc.Height = Window::screenHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;

		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		ID3D11Texture2D* pDepthStencil;

		HR(device->CreateTexture2D(&depthStencilDesc, NULL, &pDepthStencil));

		D3D11_DEPTH_STENCIL_VIEW_DESC ds;
		ZeroMemory(&ds, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		ds.Format = depthStencilDesc.Format;
		ds.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		ds.Texture2D.MipSlice = 0;

		HR(device->CreateDepthStencilView(pDepthStencil, NULL, &depthStencil));

		/* uncomment this if depth testing stops working

		D3D11_DEPTH_STENCIL_DESC dsDesc;
		ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		// Depth test parameters
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Stencil test parameters
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create depth stencil state
		HR(device->CreateDepthStencilState(&dsDesc, &depthStencilState));

		context->OMSetDepthStencilState(depthStencilState, 1);
		*/
		
		// -----

		// set the render target
		context->OMSetRenderTargets(1, &backbuffer, depthStencil);

		pBackBuffer->Release();
		pBackBuffer = NULL;

		pDepthStencil->Release();
		pDepthStencil = NULL;
	}

	void DX11Renderer::setupViewportAndCamera(Window* window) {

		if (this->camera != NULL) {
			throw std::runtime_error("You can only setup the camera and viewport once");
		}

		// describe the viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof D3D11_VIEWPORT);

		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.Width = (float) Window::screenWidth;
		viewport.Height = (float) Window::screenHeight;

		context->RSSetViewports(1, &viewport);

		// Camera and Perspective Matrices
		this->camera = new Camera(Point(0, 0, -10), Point(0, 0, 0), Vector(0, 1, 0),
			(float) M_PI / 4.0f, (float)Window::screenWidth / (float)Window::screenHeight, 1, 1000);

	}

	void DX11Renderer::setupConstantBuffer() {
		if (perFrameBuffer != NULL || perVertexBuffer != NULL || timeBuffer != NULL) {
			throw std::runtime_error("You can only setup the constant buffers once");
		}

		// setup per frame buffer
		D3D11_BUFFER_DESC cb;
		ZeroMemory(&cb, sizeof(cb));

		cb.Usage = D3D11_USAGE_DEFAULT;
		cb.ByteWidth = sizeof(float[4][4]) + sizeof(float[4]);
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.CPUAccessFlags = 0;

		HR(device->CreateBuffer(&cb, NULL, &this->perFrameBuffer));

		cb.ByteWidth = sizeof(float[4][4]);

		HR(device->CreateBuffer(&cb, NULL, &this->perVertexBuffer));

		renderTimer = new DX11Timer();

		cb.ByteWidth = sizeof(float[4]);

		HR(device->CreateBuffer(&cb, NULL, &this->timeBuffer));
	}

	void DX11Renderer::setupShaders(char* vertex, char* pixel) {
		if (defaultPixelShader != NULL || defaultVertexShader != NULL || layout != NULL) {
			throw std::runtime_error("You can only setup shaders once");
		}

		// default shaders
		this->defaultVertexShader = new DX11VertexShader(vertex, this, this->device, this->context);
		this->defaultPixelShader = new DX11PixelShader(pixel, this, this->device, this->context);

		// Input Layout for vertex buffers
		ied = new D3D11_INPUT_ELEMENT_DESC[3];
		ied[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		ied[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		ied[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 };

		//Kept the following in case something is using the default vs and ps without a model

		defaultVertexShader->set();
		defaultPixelShader->set();
	}
	
	//=============================================//
	//                   Methods                   //
	//=============================================//

	void DX11Renderer::clearFrame() {
		float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
		context->ClearRenderTargetView(backbuffer, color);
		context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// set default stuff
		float world[4][4];
		float viewProjection[5][4];
		float time[1][2];

		memcpy(world, Matrix4::identity().getPointer(), sizeof(float[4][4]));
		memcpy(viewProjection, (this->camera->getCameraInverse() * this->camera->getPerspective()).getPointer(), sizeof(float[4][4]));
		
		Vector4 cameraPos = this->camera->getPosition();
		viewProjection[4][0] = cameraPos.x();
		viewProjection[4][1] = cameraPos.y();
		viewProjection[4][2] = cameraPos.z();
		viewProjection[4][3] = 1.0f;

		renderTimer->GetElapsedTimeAndTimeSinceLastFrame(&time[0][0], &time[0][1]);

		context->UpdateSubresource(perFrameBuffer, 0, NULL, &viewProjection, 0, 0);
		context->UpdateSubresource(perVertexBuffer, 0, NULL, &world, 0, 0);
		context->UpdateSubresource(timeBuffer, 0, NULL, &time, 0, 0);

		ID3D11Buffer* cBuffers[] = { perFrameBuffer, perVertexBuffer, timeBuffer };
		context->VSSetConstantBuffers(0, 3, cBuffers);

		// set shaders without layout
		defaultVertexShader->setWithNoLayout();
		defaultPixelShader->setWithNoLayout(); //using this function for consistency
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

	Model* DX11Renderer::createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture) {
		return new DX11Model(v, i, context, texture, this);
	}

	Model* DX11Renderer::createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Shader* vs, Shader* ps) {
		return new DX11Model(v, i, context, texture, this, vs, ps);
	}

	Texture* DX11Renderer::createTextureFromFile(char* f) {
		return new DX11Texture(f, this->device, this->context);
	}

	Shader* DX11Renderer::createVertexShader(char* f) {
		return new DX11VertexShader(f, this, this->device, this->context);
	}

	Shader* DX11Renderer::createPixelShader(char* f) {
		return new DX11PixelShader(f, this, this->device, this->context);
	}

	Shader* DX11Renderer::getDefaultVertexShader() {
		return defaultVertexShader;
	}

	Shader* DX11Renderer::getDefaultPixelShader() {
		return defaultPixelShader;
	}

	ID3D11InputLayout * DX11Renderer::getLayout() {
		return layout;
	}

	ID3D11InputLayout ** DX11Renderer::getLayoutAddress() {
		return &layout;
	}

	D3D11_INPUT_ELEMENT_DESC* DX11Renderer::getInputElementDesc() {
		return ied;
	}

	Camera* DX11Renderer::getCamera() {
		return this->camera;
	}

	Timer* DX11Renderer::getTimer() {
		return this->renderTimer;
	}

	void DX11Renderer::setObjectMatrix(Matrix4 t) {
		// set default stuff
		float world[4][4];

		memcpy(world, t.transpose().getPointer(), sizeof(float[4][4]));

		context->UpdateSubresource(perVertexBuffer, 0, NULL, &world, 0, 0);
		context->VSSetConstantBuffers(1, 1, &perVertexBuffer);
	}
}
