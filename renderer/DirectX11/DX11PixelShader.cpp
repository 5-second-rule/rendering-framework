#include "DX11PixelShader.h"

#include "dxerr.h"

namespace Transmission {


	DX11PixelShader::DX11PixelShader(char* filename, ID3D11Device* device) : DX11Shader(filename) {
		HR(device->CreatePixelShader(this->bytecode, this->length, NULL, (ID3D11PixelShader**) &this->shader));
	}


	DX11PixelShader::~DX11PixelShader() {}

	ID3D11PixelShader* DX11PixelShader::getPixelShader() {
		return (ID3D11PixelShader*)this->shader;
	}

}