#include "DX11PixelShader.h"


DX11PixelShader::DX11PixelShader(char* filename, ID3D11Device* device) : DX11Shader(filename) {
	this->shader = this->createShader(this->bytecode, this->length, device);
}


DX11PixelShader::~DX11PixelShader() {}

ID3D11PixelShader* DX11PixelShader::getPixelShader() {
	return (ID3D11PixelShader*)this->shader;
}

ID3D11DeviceChild* DX11PixelShader::createShader(char* buf, size_t length, ID3D11Device* device) {
	return (ID3D11DeviceChild*)device->CreatePixelShader(buf, length, NULL, (ID3D11PixelShader**)&this->shader);
}