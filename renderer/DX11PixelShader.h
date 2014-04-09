#pragma once

#include "DX11Shader.h"

class DX11PixelShader :
	public DX11Shader
{
public:
	DX11PixelShader(char*, ID3D11Device*);
	~DX11PixelShader();

	ID3D11PixelShader* getPixelShader();
};

