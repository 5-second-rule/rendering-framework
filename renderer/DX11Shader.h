#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class DX11Shader
{
protected:
	ID3D11DeviceChild* shader;

public:
	DX11Shader(char*, ID3D11Device*);
	~DX11Shader();

	ID3D11DeviceChild* getShader();

private:
	struct buf_len {
		char* buf;
		size_t len;
	};

	buf_len readShader(char*);

protected:
	virtual ID3D11DeviceChild* createShader(char*, size_t length, ID3D11Device*) = 0;

};

