#pragma once

#include <Windows.h>
#include <d3d11.h>
#include "dxerr.h"

#pragma comment(lib, "d3d11.lib")

#include "Renderer.h"
#include "Window.h"

#define NUM_LIGHTS 10

#define DX11Factory(T) \
	template<> \
	class Renderer::Factory<T> { \
		template<typename... Args> \
		T* create(Renderer* renderer, Args... args) { \
			DX11Renderer* dx = static_cast<DX11Renderer*>(renderer); \
			return new DX11##T(dx->device, dx->context, args...); \
		} \
	}

struct LightDataBufferType
{
	lightData lightDataVals[NUM_LIGHTS];
};

namespace Transmission {

	class DX11Renderer :
		public Renderer
	{
	friend Renderer;
	protected:
		D3D_FEATURE_LEVEL featureLevel;
		UINT msaaQuality;

		ID3D11Device* device;
		ID3D11DeviceContext* context;
		IDXGISwapChain* swapchain;


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

		ID3D11Buffer* perFrameBuffer;
		ID3D11Buffer* perVertexBuffer;
		ID3D11Buffer* timeBuffer;
		ID3D11Buffer* lightDataBuffer;
		ID3D11Buffer* saturationLightnessBuffer;

		Camera* camera;

		bool windowed = true;
		
		int renderDimension;
		Window * window;

	public:
		DX11Renderer( Window* window, char* vertex, char* pixel );
		~DX11Renderer();

	protected:
		virtual void setupDeviceAndSwapChain(Window*);
		virtual void setupBackBuffer(unsigned int width, unsigned int height);
		virtual void setupAlphaBlending();
		virtual void setupShaders( char* vertex, char* pixel );
		virtual void setupConstantBuffer();
		virtual void useWorldCoords();

	public:

		virtual void resize(unsigned int width, unsigned int height, bool);
		virtual void clearFrame();

		virtual bool setLightBuffers(Common::Vector4* lightPositions, Common::Vector4* lightColors, int numLightsProvided);
		virtual void prep2D();
		virtual void end2D();
		virtual void makeTransparent();
		virtual void makeOpaque();
		virtual void turnDepthOff();
		virtual void turnDepthOn();
		virtual void useScreenCoords();
		virtual void drawFrame();

		virtual void turnDepthTestOff();
		virtual void turnDepthTestOn();

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
		virtual Model* create2DModelFromVertices(Vertex* v, int numVertices, Index* i, int numIndices, Texture* texture, bool isTransparent);
		virtual Model* create2DModelFromVertices(Vertex* v, int numVertices, Index* i, int numIndices, Texture* texture, Shader* vs, Shader* ps, bool isTransparent);

		virtual Texture* createTextureFromFile(char* f);

		virtual Shader* createVertexShader(char*);
		virtual Shader* createPixelShader(char*);

		virtual Shader* getDefaultVertexShader();
		virtual Shader* getDefaultPixelShader();

		ID3D11InputLayout * getLayout();
		ID3D11InputLayout ** getLayoutAddress();
		D3D11_INPUT_ELEMENT_DESC* getInputElementDesc();

		virtual Camera* getCamera();
		virtual void setObjectMatrix(Matrix4);
	};
}
