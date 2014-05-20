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
		ID3D11DepthStencilState * depthStencilStateDepthOff;

		ID3D11BlendState* transparency;

		Shader* defaultVertexShader;
		Shader* defaultPixelShader;
		ID3D11InputLayout *layout;
		D3D11_INPUT_ELEMENT_DESC* ied;

		wchar_t *fontString;

		Timer* renderTimer;

		ID3D11Buffer* perFrameBuffer;
		ID3D11Buffer* perVertexBuffer;
		ID3D11Buffer* timeBuffer;

		Camera* camera;

		bool windowed = true;
		
		int renderDimension;

	public:
		DX11Renderer(Window*);
		~DX11Renderer();

	protected:
		virtual void setupDeviceAndSwapChain(Window*);
		virtual void setupBackBuffer();
		virtual void setupViewportAndCamera(Window*);
		virtual void setupAlphaBlending();
		virtual void setupShaders();
		virtual void setupConstantBuffer();
		virtual void useWorldCoords();

	public:

		virtual void clearFrame();
		virtual void prep2D();
		virtual void end2D();
		virtual void makeTransparent();
		virtual void makeOpaque();
		virtual void turnDepthOff();
		virtual void turnDepthOn();
		virtual void useScreenCoords();
		virtual void drawFrame();

		virtual VertexBuffer* createVertexBuffer(Vertex [], size_t);
		virtual IndexBuffer* createIndexBuffer(unsigned int [], size_t);
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture);
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, bool is2D);
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Texture* bump);
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Shader* vs, Shader* ps);
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Shader* vs, Shader* ps, bool is2D);
		virtual Model* createModel(VertexBuffer* v, IndexBuffer* i, Texture* texture, Texture* bump, Shader* vs, Shader* ps);

		virtual Model* create2DModelFromVertices(Vertex* v, int numVertices, Index* i, int numIndices, Texture* texture);
		virtual Model* create2DModelFromVertices(Vertex* v, int numVertices, Index* i, int numIndices, Texture* texture, Shader* vs, Shader* ps);

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
