#pragma once

#include <Windows.h>
#include <d3d11.h>
#include "dxerr.h"

#pragma comment(lib, "d3d11.lib")

#include "Renderer.h"
#include "Window.h"

namespace Transmission {

	class DX11Renderer :
		public Renderer
	{
	protected:
		IDXGISwapChain* swapchain;
		ID3D11Device* device;
		ID3D11DeviceContext* context;

		ID3D11RenderTargetView* backbuffer;
		ID3D11DepthStencilView* depthStencil;
		ID3D11DepthStencilState * depthStencilState;

		Shader* defaultVertexShader;
		Shader* defaultPixelShader;
		ID3D11InputLayout *layout;
		D3D11_INPUT_ELEMENT_DESC* ied;

		Timer* renderTimer;

		ID3D11Buffer* perFrameBuffer;
		ID3D11Buffer* perVertexBuffer;
		ID3D11Buffer* timeBuffer;

		Camera* camera;

		bool windowed = true;

	public:
		DX11Renderer( Window* window, char* vertex, char* pixel );
		~DX11Renderer();

	protected:
		virtual void setupDeviceAndSwapChain(Window*);
		virtual void setupBackBuffer();
		virtual void setupViewportAndCamera(Window*);
		virtual void setupShaders( char* vertex, char* pixel );
		virtual void setupConstantBuffer();

	public:

		virtual void clearFrame();
		virtual void drawFrame();

		virtual VertexBuffer* createVertexBuffer(Vertex [], size_t);
		virtual IndexBuffer* createIndexBuffer(unsigned int [], size_t);
		virtual Model* createModel(VertexBuffer*, IndexBuffer*, Texture*);
		virtual Model* createModel(VertexBuffer*, IndexBuffer*, Texture*, Shader*, Shader*);
		virtual Texture* createTextureFromFile(char* f);

		virtual Shader* createVertexShader(char*);
		virtual Shader* createPixelShader(char*);

		virtual Shader* getDefaultVertexShader();
		virtual Shader* getDefaultPixelShader();

		ID3D11InputLayout * getLayout();
		ID3D11InputLayout ** getLayoutAddress();
		D3D11_INPUT_ELEMENT_DESC* getInputElementDesc();

		virtual Camera* getCamera();
		virtual Timer* getTimer();
		virtual void setObjectMatrix(Matrix4);
	};
}
