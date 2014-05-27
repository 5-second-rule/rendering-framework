#pragma once
#include "DX11Shader.h"

namespace Transmission {

	class DX11VertexShader :
		public DX11Shader
	{
	public:
		DX11VertexShader(char*, DX11Renderer*, ID3D11Device*, ID3D11DeviceContext*);
		~DX11VertexShader();

		ID3D11VertexShader* getVertexShader();
		
		virtual void set();
		virtual void setWithNoLayout();
	};

}