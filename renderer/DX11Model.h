#pragma once

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include "Model.h"

#include "DX11VertexBuffer.h"
#include "DX11IndexBuffer.h"

class DX11Model :
	public Model
{
private:
	ID3D11DeviceContext* context;

public:
	DX11Model(VertexBuffer*, IndexBuffer*, ID3D11DeviceContext*);
	~DX11Model();

	virtual void draw();
};

