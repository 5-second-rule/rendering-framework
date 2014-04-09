#include "DX11VertexShader.h"


DX11VertexShader::DX11VertexShader(char* filename, ID3D11Device* device) : DX11Shader(filename) {
	this->shader = this->createShader(this->bytecode, this->length, device);
}


DX11VertexShader::~DX11VertexShader() {}

ID3D11VertexShader* DX11VertexShader::getVertexShader() {
	return (ID3D11VertexShader*)this->shader;
}

ID3D11DeviceChild* DX11VertexShader::createShader(char* buf, size_t length, ID3D11Device* device) {
	return (ID3D11DeviceChild*)device->CreateVertexShader(buf, length, NULL, (ID3D11VertexShader**)&this->shader);
}