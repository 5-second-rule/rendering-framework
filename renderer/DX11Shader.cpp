#include "DX11Shader.h"

#include <iostream>
#include <fstream>


DX11Shader::DX11Shader(char* filename) {
	std::ifstream shader(filename, std::ifstream::binary);

	shader.seekg(0, shader.end);
	this->length = (size_t)shader.tellg();
	shader.seekg(0, shader.beg);

	this->bytecode = new char[this->length];

	shader.read(this->bytecode, this->length);
	shader.close();
}


DX11Shader::~DX11Shader() {
	this->shader->Release();
}

ID3D11DeviceChild* DX11Shader::getShader() {
	return this->shader;
}

DX11Shader::Buffer DX11Shader::getBytecode() {
	return{ this->bytecode, this->length };
}