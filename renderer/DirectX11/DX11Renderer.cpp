#define _USE_MATH_DEFINES
#include "DX11Renderer.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "Matrix4.h"
#include "Vector4.h"

#include "DX11VertexBuffer.h"
#include "DX11IndexBuffer.h"
#include "DX11Model.h"
#include "DX11Texture.h"

#define USE_MSAA true

namespace Transmission {


	DX11Renderer::DX11Renderer(Window* window) : Renderer()
	{
		swapchain = NULL; device = NULL; context = NULL;
		this->setupDeviceAndSwapChain(window);

		backbuffer = NULL;
		this->setupBackBuffer(window);

		camera = NULL;
		this->setupViewportAndCamera(window);
		/* ---------- */

		perFrameBuffer = NULL; perVertexBuffer = NULL;
		this->setupConstantBuffer();

		vertexShader = NULL; pixelShader = NULL; layout = NULL;
		this->setupShaders();
	}


	DX11Renderer::~DX11Renderer()
	{
		delete camera;

		swapchain->SetFullscreenState(false, NULL);

		delete vertexShader;
		delete pixelShader;

		swapchain->Release();
		backbuffer->Release();
		perFrameBuffer->Release();
		perVertexBuffer->Release();
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


		UINT createDeviceFlags = 0;

		#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif


		D3D_FEATURE_LEVEL featureLevels [] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3
		};

		HR(D3D11CreateDevice(
			NULL, //default adapter
			D3D_DRIVER_TYPE_HARDWARE,
			0, //no software device
			createDeviceFlags,
			featureLevels, 4,
			D3D11_SDK_VERSION,
			&this->device,
			&this->featureLevel,
			&this->context
		));

		if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
			MessageBoxA(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
		}

		UINT msaauality;
		HR(device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &this->msaaQuality));
		
		if (!(this->msaaQuality > 0)) {
			MessageBoxA(0, "4x MSAA unsupported", 0, 0);
		}


		// Create a descriptor for our swap chain
		DXGI_SWAP_CHAIN_DESC desc;

		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

		desc.BufferCount = 1;
		desc.BufferDesc.Width = window->getWidth();
		desc.BufferDesc.Height = window->getHeight();
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.OutputWindow = (HWND) (window->getHandle());
		if (USE_MSAA && this->msaaQuality > 0) {
			desc.SampleDesc.Count = 4;
			desc.SampleDesc.Quality = this->msaaQuality - 1;
		} else {
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
		}
		desc.Windowed = this->windowed; //we should have a variable in config, for now its in header
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


		IDXGIDevice* dxgiDevice = NULL;
		HR(device->QueryInterface(__uuidof(IDXGIDevice), (void**) &dxgiDevice));

		IDXGIAdapter* dxgiAdapter = NULL;
		HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**) &dxgiAdapter));

		IDXGIFactory* dxgiFactory = NULL;
		HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**) &dxgiFactory));

		HR(dxgiFactory->CreateSwapChain(this->device, &desc, &this->swapchain));

		dxgiDevice->Release();
		dxgiAdapter->Release();
		dxgiFactory->Release();

	}

	void DX11Renderer::setupBackBuffer(Window* window) {

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

		depthStencilDesc.Width = window->getWidth();
		depthStencilDesc.Height = window->getHeight();
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		if (USE_MSAA && this->msaaQuality > 0) {
			depthStencilDesc.SampleDesc.Count = 4;
			depthStencilDesc.SampleDesc.Quality = this->msaaQuality - 1;
		} else {
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
		}

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
		viewport.Width = window->getWidth();
		viewport.Height = window->getHeight();

		context->RSSetViewports(1, &viewport);

		// Camera and Perspective Matrices
		this->camera = new Camera(Point(0, 5, -10), Point(0, 5, 0), Vector(0, 1, 0),
			(float)M_PI / 4.0f, (float)window->getWidth() / (float)window->getHeight(), 1, 1000);

	}

	void DX11Renderer::setupConstantBuffer() {
		if (perFrameBuffer != NULL || perVertexBuffer != NULL) {
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


	}

	void DX11Renderer::setupShaders() {
		if (pixelShader != NULL || vertexShader != NULL || layout != NULL) {
			throw std::runtime_error("You can only setup shaders once");
		}

		// fixed shaders for now
		this->vertexShader = new DX11VertexShader("vertex.cso", this->device);
		this->pixelShader = new DX11PixelShader("pixel.cso", this->device);

		// Input Layout for vertex buffers
		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		DX11Shader::Buffer VSbytecode = vertexShader->getBytecode();
		HR(device->CreateInputLayout(ied, 3, VSbytecode.buf, VSbytecode.len, &this->layout));
		context->IASetInputLayout(this->layout);

		// set shaders
		context->VSSetShader(vertexShader->getVertexShader(), NULL, 0);
		context->PSSetShader(pixelShader->getPixelShader(), NULL, 0);
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

		memcpy(world, Matrix4::identity().getPointer(), sizeof(float[4][4]));
		memcpy(viewProjection, (this->camera->getCameraInverse() * this->camera->getPerspective()).getPointer(), sizeof(float[4][4]));
		
		Vector4 cameraPos = this->camera->getPosition();
		viewProjection[4][0] = cameraPos.x();
		viewProjection[4][1] = cameraPos.y();
		viewProjection[4][2] = cameraPos.z();
		viewProjection[4][3] = 1.0f;


		context->UpdateSubresource(perFrameBuffer, 0, NULL, &viewProjection, 0, 0);
		context->UpdateSubresource(perVertexBuffer, 0, NULL, &world, 0, 0);

		ID3D11Buffer* cBuffers [] = { perFrameBuffer, perVertexBuffer };
		context->VSSetConstantBuffers(0, 2, cBuffers);

		// set shaders
		context->VSSetShader(vertexShader->getVertexShader(), NULL, 0);
		context->PSSetShader(pixelShader->getPixelShader(), NULL, 0);
	}

	void DX11Renderer::drawFrame() {

		swapchain->Present(0, 0);
	}

	VertexBuffer* DX11Renderer::createVertexBuffer(Vertex vertices [], size_t num) {
		return new DX11VertexBuffer(vertices, num, this->device, this->context);
	}

	IndexBuffer* DX11Renderer::createIndexBuffer(unsigned int indices [], size_t num) {
		return new DX11IndexBuffer(indices, num, this->device, this->context);
	}
Model* DX11Renderer::createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture) {
	return new DX11Model(v, i, context, texture, this);
}

Texture* DX11Renderer::createTextureFromFile(char* f) {
	return new DX11Texture(f, this->device, this->context);
}

	Camera* DX11Renderer::getCamera() {
		return this->camera;
	}

	void DX11Renderer::setObjectMatrix(Matrix4 t) {
		// set default stuff
		float world[4][4];

		memcpy(world, t.transpose().getPointer(), sizeof(float[4][4]));

		context->UpdateSubresource(perVertexBuffer, 0, NULL, &world, 0, 0);
		context->VSSetConstantBuffers(1, 1, &perVertexBuffer);
	}
}
