#include "DX11PixelShader.h"

#include <stdexcept>

DX11PixelShader::DX11PixelShader(char* filename, ID3D11Device* device) : DX11Shader(filename) {
	if (FAILED(device->CreatePixelShader(this->bytecode, this->length, NULL, (ID3D11PixelShader**)&this->shader)))
		throw std::runtime_error("Could not create pixel shader");
}


DX11PixelShader::~DX11PixelShader() {}

ID3D11PixelShader* DX11PixelShader::getPixelShader() {
	return (ID3D11PixelShader*)this->shader;
}