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
	public: \
		template<typename... Args> \
		static T* create(Renderer* renderer, Args... args) { \
			DX11Renderer* dx = static_cast<DX11Renderer*>(renderer); \
			return new DX11##T(dx->device, dx->context, args...); \
		} \
	}

namespace Transmission {

	class DX11Renderer : public Renderer
	{
	friend Renderer;

	public:
		DX11Renderer( Window* window );
		~DX11Renderer();

		virtual void resize(unsigned int width, unsigned int height, bool) override;
		virtual void clearFrame() override;

		virtual void turnDepthOff() override;
		virtual void turnDepthOn() override;
		virtual void drawFrame() override;

		virtual Camera* getCamera() override;
		virtual void setObjectMatrix(Matrix4) override;

	protected:
		
		virtual void setupDeviceAndSwapChain(Window*);
		virtual void setupBackBuffer(unsigned int width, unsigned int height);
		virtual void setupAlphaBlending();
		virtual void setupConstantBuffer();

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
	};
}
