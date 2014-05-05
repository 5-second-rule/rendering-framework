#include "DX11PixelShader.h"

#include "dxerr.h"

namespace Transmission {


	DX11PixelShader::DX11PixelShader(char* filename, DX11Renderer* renderer, ID3D11Device* device, ID3D11DeviceContext* context) : DX11Shader(filename) {
		this->renderer = renderer;
		this->device = device;
		this->context = context;
		HR(device->CreatePixelShader(this->bytecode, this->length, NULL, (ID3D11PixelShader**)&this->shader));
	}

	DX11PixelShader::~DX11PixelShader() {}

	ID3D11PixelShader* DX11PixelShader::getPixelShader() {
		return (ID3D11PixelShader*)this->shader;
	}

	void DX11PixelShader::set() {
		context->PSSetShader(this->getPixelShader(), NULL, 0);
	}

	//Created since vertex shader needed a no layout function
	void DX11PixelShader::setWithNoLayout() {
		context->PSSetShader(this->getPixelShader(), NULL, 0);
	}

}