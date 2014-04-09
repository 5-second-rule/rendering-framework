#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class DX11Shader
{
protected:
	ID3D11DeviceChild* shader;

	char* bytecode;
	size_t length;

public:
	DX11Shader(char*);
	~DX11Shader();

	ID3D11DeviceChild* getShader();

	struct Buffer {
		char* buf;
		size_t len;
	};

	Buffer getBytecode();

protected:

	virtual ID3D11DeviceChild* createShader(char*, size_t, ID3D11Device*) = 0;

};

