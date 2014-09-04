#pragma once
#include "Shader.h"
#include "DX11Shader.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

namespace Transmission {

	class DX11VertexShader :
		public DX11Shader, public VertexShader
	{
	public:
		DX11VertexShader(ID3D11Device* device, ID3D11DeviceContext* context, char* filename);

		ID3D11VertexShader* getVertexShader();
		
		virtual void set();
		virtual void setWithNoLayout();

	private:
		ID3D11InputLayout* layout;
	};

	DX11Factory(VertexShader);
}