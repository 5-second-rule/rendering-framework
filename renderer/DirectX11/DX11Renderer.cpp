#define _USE_MATH_DEFINES
#include "DX11Renderer.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "common/Matrix4.h"
#include "common/Vector4.h"
using namespace Common;

#include "DX11VertexBuffer.h"
#include "DX11IndexBuffer.h"
#include "DX11Model.h"
#include "DX11Model2D.h"
#include "DX11Texture.h"
#include "DX11VertexShader.h"
#include "DX11PixelShader.h"
#include "DX11Timer.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#define USE_MSAA true

namespace Transmission {


	DX11Renderer::DX11Renderer(Window* window, char* vertex, char* pixel) : Renderer()
	{
		swapchain = NULL; device = NULL; context = NULL;
		this->setupDeviceAndSwapChain(window);

		backbuffer = NULL;
		this->setupBackBuffer(window->getWidth(), window->getHeight());

		// Camera and Perspective Matrices
		this->camera = new Camera(Point(0, 0, -10), Point(0, 0, 0), Vector(0, 1, 0),
			(float)M_PI / 4.0f, (float)window->getWidth() / (float)window->getHeight(), 1, 10000);

		/* ---------- */

		this->saturation = 0;
		this->lightness = 0;

		this->setupAlphaBlending();

		renderTimer = NULL;
		perFrameBuffer = NULL; perVertexBuffer = NULL; timeBuffer = NULL; lightDataBuffer = NULL; saturationLightnessBuffer = NULL;
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
		layout->Release();
		transparency->Release();
		depthStencil->Release();
		depthStencilState->Release();
		depthStencilStateDepthOff->Release();
		perFrameBuffer->Release();
		perVertexBuffer->Release();
		timeBuffer->Release();
		lightDataBuffer->Release();
		saturationLightnessBuffer->Release();
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
		
		HR(dxgiFactory->MakeWindowAssociation((HWND)window->getHandle(), DXGI_MWA_NO_WINDOW_CHANGES));

		dxgiDevice->Release();
		dxgiAdapter->Release();
		dxgiFactory->Release();

	}

	void DX11Renderer::setupBackBuffer(unsigned int width, unsigned int height) {

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

		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
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

		// uncomment this if depth testing stops working

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

		// Depth test parameters
		dsDesc.DepthEnable = false;

		// Create depth stencil state
		HR(device->CreateDepthStencilState(&dsDesc, &depthStencilStateDepthOff));



		context->OMSetDepthStencilState(depthStencilState, 1);
		
		
		// -----

		// set the render target
		context->OMSetRenderTargets(1, &backbuffer, depthStencil);

		pBackBuffer->Release();
		pBackBuffer = NULL;

		pDepthStencil->Release();
		pDepthStencil = NULL;

		// describe the viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof D3D11_VIEWPORT);

		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;

		context->RSSetViewports(1, &viewport);
	}

	void DX11Renderer::setupConstantBuffer() {
		if (perFrameBuffer != NULL || perVertexBuffer != NULL || timeBuffer != NULL || lightDataBuffer != NULL || saturationLightnessBuffer != NULL) {
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

		cb.Usage = D3D11_USAGE_DYNAMIC;
		cb.ByteWidth = sizeof(LightDataBufferType);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HR(device->CreateBuffer(&cb, NULL, &this->lightDataBuffer));

		cb.Usage = D3D11_USAGE_DEFAULT;
		cb.ByteWidth = sizeof(float[4]);
		cb.CPUAccessFlags = 0;

		HR(device->CreateBuffer(&cb, NULL, &this->saturationLightnessBuffer));
	}

	void DX11Renderer::setupAlphaBlending() {
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));

		D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
		ZeroMemory(&renderTargetBlendDesc, sizeof(renderTargetBlendDesc));

		renderTargetBlendDesc.BlendEnable = true;
		renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		renderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.RenderTarget[0] = renderTargetBlendDesc;

		HR(device->CreateBlendState(&blendDesc, &transparency));
	}

	void DX11Renderer::setupShaders(char* vertex, char* pixel) {
		if (defaultPixelShader != NULL || defaultVertexShader != NULL || layout != NULL) {
			throw std::runtime_error("You can only setup shaders once");
		}

		// default shaders
		this->defaultVertexShader = new DX11VertexShader(vertex, this, this->device, this->context);
		this->defaultPixelShader = new DX11PixelShader(pixel, this, this->device, this->context);

		// Input Layout for vertex buffers
		ied = new D3D11_INPUT_ELEMENT_DESC[4];
		ied[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		ied[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		ied[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		ied[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 };

		//Kept the following in case something is using the default vs and ps without a model

		defaultVertexShader->set();
		defaultPixelShader->set();
	}
	
	//=============================================//
	//                   Methods                   //
	//=============================================//

	void DX11Renderer::resize(unsigned int width, unsigned int height, bool fullscreen) {
		
		HR(swapchain->SetFullscreenState(fullscreen, NULL));

		context->OMSetRenderTargets(0, 0, 0);

		backbuffer->Release();
		backbuffer = NULL;

		// automagically get the width and high from the stored HWND
		HR(swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0));

		this->setupBackBuffer(width, height);

		camera->resize((float)width / (float)height);
		

	}

	void DX11Renderer::clearFrame() {
		float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
		context->ClearRenderTargetView(backbuffer, color);
		context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		this->useWorldCoords();
	}

	void DX11Renderer::prep2D() {
		if (this->renderDimension != Dimension::TWO) {
			this->useScreenCoords();
			this->turnDepthOff();
			this->renderDimension = Dimension::TWO;
		}
	}

	void DX11Renderer::end2D() {
		if (this->renderDimension == Dimension::TWO) {
			this->turnDepthOn();
			this->useWorldCoords();
			this->renderDimension = Dimension::THREE;
		}
	}

	void DX11Renderer::makeTransparent() {
		context->OMSetDepthStencilState(depthStencilStateDepthOff, 1);

		float blendFactor[] = { 0.00f, 0.00f, 0.00f, 1.0f };

		context->OMSetBlendState(transparency, blendFactor, 0xffffffff);
	}

	void DX11Renderer::makeOpaque() {
		context->OMSetDepthStencilState(depthStencilState, 1);

		context->OMSetBlendState(0, 0, 0xffffffff);
	}

	void DX11Renderer::turnDepthOff() {
		context->OMSetDepthStencilState(depthStencilStateDepthOff, 1);
	}

	void DX11Renderer::turnDepthOn() {
		context->OMSetDepthStencilState(depthStencilState, 1);
	}

	void DX11Renderer::useScreenCoords() {
		// set default stuff
		float world[4][4];
		float viewProjection[5][4];

		memcpy(world, Matrix4::identity().getPointer(), sizeof(float[4][4]));
		memcpy(viewProjection, Matrix4::identity().getPointer(), sizeof(float[4][4]));

		Vector4 cameraPos = this->camera->getPosition();
		viewProjection[4][0] = cameraPos.x();
		viewProjection[4][1] = cameraPos.y();
		viewProjection[4][2] = cameraPos.z();
		viewProjection[4][3] = 1.0f;

		context->UpdateSubresource(perFrameBuffer, 0, NULL, &viewProjection, 0, 0);
		context->UpdateSubresource(perVertexBuffer, 0, NULL, &world, 0, 0);
	}

	void DX11Renderer::useWorldCoords() {
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

		float saturationLightness[2];
		saturationLightness[0] = this->saturation;
		saturationLightness[1] = this->lightness;

		context->UpdateSubresource(saturationLightnessBuffer, 0, NULL, &saturationLightness, 0, 0);

		context->PSSetConstantBuffers(1, 1, &saturationLightnessBuffer);

		// set shaders without layout
		defaultVertexShader->setWithNoLayout();
		defaultPixelShader->setWithNoLayout(); //using this function for consistency
	}

	bool DX11Renderer::setLightBuffers(Common::Vector4* lightPositions, Common::Vector4* lightColors, int numLightsProvided)
	{

		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		LightDataBufferType* dataPtr;

		// Lock the light position constant buffer so it can be written to.
		result = context->Map(lightDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		dataPtr = (LightDataBufferType*)mappedResource.pData;

		for (int i = 0; i < NUM_LIGHTS; i++)
		{
			if (i < numLightsProvided)
			{
				Common::Vector4 updatePosition;

				//Just needs world, why is world identity?
				updatePosition = ((Matrix4::identity()*lightPositions[i]));

				memcpy(dataPtr->lightDataVals[i].position, updatePosition.getPointer(), 4*sizeof(float));

				memcpy(dataPtr->lightDataVals[i].color, lightColors[i].getPointer(), 4 * sizeof(float));
			}
			else
			{
				//assign w value to 0 to specify light isn't on
				dataPtr->lightDataVals[i].position[3] = 0;
			}
		}
		
		// Unlock the constant buffer.
		context->Unmap(lightDataBuffer, 0);

		context->PSSetConstantBuffers(0, 1, &lightDataBuffer);

		return true;
	}

	void DX11Renderer::drawFrame() {
		swapchain->Present(0, 0);
	}

	/* Turns the depth testing off
	 - Note: Once finished with using no depth, should be followed by turnDepthTestOn()
	*/
	void DX11Renderer::turnDepthTestOff() {
		context->OMSetDepthStencilState(depthStencilStateDepthOff, 1);
	}

	/* Turns the depth testing back on
	*/
	void DX11Renderer::turnDepthTestOn() {
		context->OMSetDepthStencilState(depthStencilState, 1);
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

	Model* DX11Renderer::createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, bool is2D) {
		if (is2D)
			return new DX11Model2D(v, i, context, texture, this);
		
		return new DX11Model(v, i, context, texture, this);
	}

	Model* DX11Renderer::createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Texture* bump) {
		return new DX11Model(v, i, context, texture, bump, this);
	}

	Model* DX11Renderer::createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Shader* vs, Shader* ps) {
		return new DX11Model(v, i, context, texture, this, vs, ps);
	}

	Model* DX11Renderer::createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Shader* vs, Shader* ps, bool is2D) {
		if (is2D)
			return new DX11Model2D(v, i, context, texture, this, vs, ps);

		return new DX11Model(v, i, context, texture, this, vs, ps);
	}

	Model* DX11Renderer::createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Texture* bump, Shader* vs, Shader* ps) {
		return new DX11Model(v, i, context, texture, bump, this, vs, ps);
	}

	Model* DX11Renderer::create2DModelFromVertices(Vertex* v, int numVertices, Index* i, int numIndices, Texture* texture) {
		return new DX11Model2D(v, numVertices, i, numIndices, this->context, texture, this);
	}

	Model* DX11Renderer::create2DModelFromVertices(Vertex* v, int numVertices, Index* i, int numIndices, Texture* texture, Shader* vs, Shader* ps) {
		return new DX11Model2D(v, numVertices, i, numIndices, this->context, texture, this, vs, ps);
	}

	Model* DX11Renderer::create2DModelFromVertices(Vertex* v, int numVertices, Index* i, int numIndices, Texture* texture, bool isTransparent) {
		return new DX11Model2D(v, numVertices, i, numIndices, this->context, texture, this, isTransparent);
	}

	Model* DX11Renderer::create2DModelFromVertices(Vertex* v, int numVertices, Index* i, int numIndices, Texture* texture, Shader* vs, Shader* ps, bool isTransparent) {
		return new DX11Model2D(v, numVertices, i, numIndices, this->context, texture, this, vs, ps, isTransparent);
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
