#include "DX11Shader.h"

#include <iostream>
#include <fstream>


DX11Shader::DX11Shader(char* filename, ID3D11Device* device) {
	buf_len buf = readShader(filename);
	this->shader = createShader(buf.buf, buf.len, device);
	delete[] buf.buf;
}


DX11Shader::~DX11Shader() {
	this->shader->Release();
}

DX11Shader::buf_len DX11Shader::readShader(char* filename) {
	std::ifstream shader(filename, std::ifstream::binary);

	shader.seekg(0, shader.end);
	int length = shader.tellg();
	shader.seekg(0, shader.beg);

	char* buf = new char[length];

	shader.read(buf, length);
	shader.close();

	return { buf, length };
}

ID3D11DeviceChild* DX11Shader::getShader() {
	return this->shader;
}
