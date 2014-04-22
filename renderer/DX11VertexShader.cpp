#include "DX11VertexShader.h"

#include "DirectX11\dxerr.h"

namespace Transmission {

	DX11VertexShader::DX11VertexShader(char* filename, ID3D11Device* device) : DX11Shader(filename) {
		HR(device->CreateVertexShader(this->bytecode, this->length, NULL, (ID3D11VertexShader**) &this->shader));
	}


	DX11VertexShader::~DX11VertexShader() {}

	ID3D11VertexShader* DX11VertexShader::getVertexShader() {
		return (ID3D11VertexShader*)this->shader;
	}
}