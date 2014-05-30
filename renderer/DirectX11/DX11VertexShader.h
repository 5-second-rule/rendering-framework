#pragma once
#include "DX11Shader.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

namespace Transmission {

	class DX11VertexShader :
		public DX11Shader
	{
	public:
		DX11VertexShader(char*, DX11Renderer*, ID3D11Device*, ID3D11DeviceContext*);

		ID3D11VertexShader* getVertexShader();
		
		virtual void set();
		virtual void setWithNoLayout();
	};

}