#include "DX11PixelShader.h"

#include "dxerr.h"

namespace Transmission {


	DX11PixelShader::DX11PixelShader(DX11Renderer* renderer, const char* filename)
		: DX11Shader(filename, renderer->getContext())
	{
		HR(renderer->getDevice()->CreatePixelShader(
			this->bytecode, this->length, nullptr,
			(ID3D11PixelShader**)(&this->shader)
		));
	}

	ID3D11PixelShader* DX11PixelShader::getPixelShader() {
		return (ID3D11PixelShader*)this->shader;
	}

	void DX11PixelShader::set() {
		context->PSSetShader(this->getPixelShader(), NULL, 0);
	}

}