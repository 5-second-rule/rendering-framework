#pragma once

#include <Windows.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include "Renderer.h"
#include "Window.h"
#include "DX11VertexShader.h"
#include "DX11PixelShader.h"

class DX11Renderer :
	public Renderer
{
protected:
	IDXGISwapChain* swapchain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* backbuffer;

	ID3D11Buffer* constantBuffer;

	DX11VertexShader* vertexShader;
	DX11PixelShader* pixelShader;

	ID3D11InputLayout *layout;

	Camera* camera;

	bool windowed = true;

public:
	DX11Renderer(Window*);
	~DX11Renderer();

	virtual void clearFrame();
	virtual void drawFrame();

	virtual VertexBuffer* createVertexBuffer(Vertex[], size_t);
	virtual IndexBuffer* createIndexBuffer(unsigned int[], size_t);
	virtual Model* createModel(VertexBuffer*, IndexBuffer*);

	virtual Camera* getCamera();
	virtual void setObjectMatrix(ITransformable*);
};

