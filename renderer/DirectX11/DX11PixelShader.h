#pragma once

#include "DX11Shader.h"

namespace Transmission {

	class DX11PixelShader :
		public DX11Shader
	{
	public:
		DX11PixelShader(char*, DX11Renderer*, ID3D11Device*, ID3D11DeviceContext*);

		ID3D11PixelShader* getPixelShader();

		virtual void set();
		virtual void setWithNoLayout();
	};
}
