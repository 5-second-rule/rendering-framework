#pragma once
#include "IndexBuffer.h"

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include "util.h"

class DX11IndexBuffer :
	public IndexBuffer
{
private:
	ID3D11Buffer* buffer;

public:
	DX11IndexBuffer(unsigned int[], size_t, ID3D11Device*, ID3D11DeviceContext*);
	~DX11IndexBuffer();
};

