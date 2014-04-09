#pragma once
#include "VertexBuffer.h"

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include "util.h"


class DX11VertexBuffer :
	public VertexBuffer
{
private:
	ID3D11Buffer* buffer;

public:
	DX11VertexBuffer(Vertex[], size_t, ID3D11Device*, ID3D11DeviceContext*);
	~DX11VertexBuffer();
};

