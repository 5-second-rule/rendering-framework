#pragma once

#include "Shader.h"
#include "DX11Shader.h"
#include "DX11Renderer.h"

namespace Transmission {

	class FRAMEWORKDLL DX11PixelShader :
		public DX11Shader, public PixelShader
	{
	public:
		DX11PixelShader(DX11Renderer* renderer, const char* filename);

		ID3D11PixelShader* getPixelShader();

		virtual void set() override;
	};

	DX11Factory(PixelShader);
}
