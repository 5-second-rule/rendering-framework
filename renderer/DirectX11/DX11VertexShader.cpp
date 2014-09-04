#include "DX11VertexShader.h"

#include "dxerr.h"

namespace Transmission {

	DX11VertexShader::DX11VertexShader(ID3D11Device* device, ID3D11DeviceContext* context, char* filename) : DX11Shader(filename) {
		this->context = context;
		HR(device->CreateVertexShader(this->bytecode, this->length, NULL, (ID3D11VertexShader**)&this->shader));

		//TODO: find a better way to handle input layout (reflection?)
		static D3D11_INPUT_ELEMENT_DESC ied [] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		HR(device->CreateInputLayout(ied, 4, this->bytecode, this->length, &this->layout));
	}

	ID3D11VertexShader* DX11VertexShader::getVertexShader() {
		return (ID3D11VertexShader*)this->shader;
	}

	void DX11VertexShader::set()
	{
		context->IASetInputLayout(this->layout);
		context->VSSetShader(this->getVertexShader(), NULL, 0);
	}

	//Created for the clear frame in the Renderer
	void DX11VertexShader::setWithNoLayout()
	{
		context->VSSetShader(this->getVertexShader(), NULL, 0);
	}

}