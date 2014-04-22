#include "DX11VertexShader.h"

#include <stdexcept>

namespace Transmission {

	DX11VertexShader::DX11VertexShader(char* filename, ID3D11Device* device) : DX11Shader(filename) {
		if (FAILED(device->CreateVertexShader(this->bytecode, this->length, NULL, (ID3D11VertexShader**) &this->shader)))
			throw std::runtime_error("Could not create vertex shader");
	}


	DX11VertexShader::~DX11VertexShader() {}

	ID3D11VertexShader* DX11VertexShader::getVertexShader() {
		return (ID3D11VertexShader*)this->shader;
	}
}