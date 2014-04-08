#pragma once
#include "DX11Shader.h"
class DX11VertexShader :
	public DX11Shader
{
public:
	DX11VertexShader(char*, ID3D11Device*);
	~DX11VertexShader();

	ID3D11VertexShader* getVertexShader();

protected:
	virtual ID3D11DeviceChild* createShader(char*, size_t, ID3D11Device*);
};

