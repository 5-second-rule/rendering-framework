#pragma once

#include <Windows.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include "Renderer.h"
#include "Window.h"

class DX11Renderer :
	public Renderer
{
protected:
	IDXGISwapChain* swapchain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* backbuffer;

public:
	DX11Renderer(Window&);
	~DX11Renderer();

	virtual void initializePipeline();
	virtual void renderFrame();
};

