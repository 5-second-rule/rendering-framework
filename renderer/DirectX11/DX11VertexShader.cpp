#include "DX11VertexShader.h"

#include "dxerr.h"

namespace Transmission {

	DX11VertexShader::DX11VertexShader(char* filename, DX11Renderer* renderer, ID3D11Device* device, ID3D11DeviceContext* context) : DX11Shader(filename) {
		this->renderer = renderer;
		this->device = device;
		this->context = context;
		HR(device->CreateVertexShader(this->bytecode, this->length, NULL, (ID3D11VertexShader**)&this->shader));
	}

	ID3D11VertexShader* DX11VertexShader::getVertexShader() {
		return (ID3D11VertexShader*)this->shader;
	}

	void DX11VertexShader::set()
	{
		DX11Shader::Buffer VSbytecode = this->getBytecode();
		HR(device->CreateInputLayout(renderer->getInputElementDesc(), 4, VSbytecode.buf, VSbytecode.len, renderer->getLayoutAddress()));
		context->IASetInputLayout(renderer->getLayout());
		context->VSSetShader(this->getVertexShader(), NULL, 0);
	}

	//Created for the clear frame in the Renderer
	void DX11VertexShader::setWithNoLayout()
	{
		context->VSSetShader(this->getVertexShader(), NULL, 0);
	}

}