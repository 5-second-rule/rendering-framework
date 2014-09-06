#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "DX11Renderer.h"

namespace Transmission {

	class FRAMEWORKDLL DX11Shader
	{
	public:
		DX11Shader(const char* filename, ID3D11DeviceContext* context);
		virtual ~DX11Shader();

		ID3D11DeviceChild* getShader();

		struct Buffer {
			char* buf;
			size_t len;
		};

		Buffer getBytecode();

	protected:
		ID3D11DeviceChild* shader;

		char* bytecode;
		size_t length;

		ID3D11DeviceContext* context;
	};

}