#pragma once

#include <Windows.h>
#include <d3d11.h>
#include "dxerr.h"

#pragma comment(lib, "d3d11.lib")

#include "Renderer.h"
#include "Window.h"
#include "DX11VertexShader.h"
#include "DX11PixelShader.h"

namespace Transmission {

	class DX11Renderer :
		public Renderer
	{
	protected:
		D3D_FEATURE_LEVEL featureLevel;
		UINT msaaQuality;

		ID3D11Device* device;
		ID3D11DeviceContext* context;
		IDXGISwapChain* swapchain;


		ID3D11RenderTargetView* backbuffer;
		ID3D11DepthStencilView* depthStencil;
		ID3D11DepthStencilState * depthStencilState;

		DX11VertexShader* vertexShader;
		DX11PixelShader* pixelShader;
		ID3D11InputLayout *layout;

		ID3D11Buffer* perFrameBuffer;
		ID3D11Buffer* perVertexBuffer;


		Camera* camera;

		bool windowed = true;

	public:
		DX11Renderer(Window*);
		~DX11Renderer();

	protected:
		virtual void setupDeviceAndSwapChain(Window*);
		virtual void setupBackBuffer(unsigned int width, unsigned int height);
		virtual void setupShaders();
		virtual void setupConstantBuffer();

	public:

		virtual void resize(unsigned int width, unsigned int height);

		virtual void clearFrame();
		virtual void drawFrame();

		virtual VertexBuffer* createVertexBuffer(Vertex [], size_t);
		virtual IndexBuffer* createIndexBuffer(unsigned int [], size_t);
		virtual Model* createModel(VertexBuffer*, IndexBuffer*, Texture*);
		virtual Texture* createTextureFromFile(char* f);


		virtual Camera* getCamera();
		virtual void setObjectMatrix(Matrix4);
	};
}
