#include "DX11Shader.h"

#include <iostream>
#include <fstream>

namespace Transmission {

	DX11Shader::DX11Shader(char* filename) : Shader(filename) {
		std::ifstream shader(filename, std::ifstream::binary);
		if (!shader) {
			TCHAR pwd[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, pwd);
			MessageBox(NULL, pwd, pwd, 0);
			MessageBoxA(NULL, strerror(errno), "Shader Error", MB_OK | MB_ICONERROR);
			exit(-1);
		}

		shader.seekg(0, shader.end);
		this->length = (size_t) shader.tellg();
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
}