#pragma once

#include "Shader.h"
#include "DX11Shader.h"

namespace Transmission {

	class DX11PixelShader :
		public DX11Shader, public PixelShader
	{
	public:
		DX11PixelShader(ID3D11Device* device, ID3D11DeviceContext* context, char* filename);

		ID3D11PixelShader* getPixelShader();

		virtual void set();
		virtual void setWithNoLayout();
	};

	DX11Factory(PixelShader);
}
