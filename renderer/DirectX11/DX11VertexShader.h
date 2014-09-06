#pragma once
#include "Shader.h"
#include "DX11Shader.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

namespace Transmission {

	class FRAMEWORKDLL DX11VertexShader :
		public DX11Shader, public VertexShader
	{
	public:
		DX11VertexShader(DX11Renderer* renderer, const char* filename);

		ID3D11VertexShader* getVertexShader();
		
		virtual void set() override;

	private:
		ID3D11InputLayout* layout;
	};

	DX11Factory(VertexShader);
}