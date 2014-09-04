#pragma once
#include "VertexBuffer.h"

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include "util.h"

#include "Renderer.h"
#include "DX11Renderer.h"

namespace Transmission {
	
	class FRAMEWORKDLL DX11VertexBuffer :
		public VertexBuffer
	{
	private:
		ID3D11Buffer* buffer;
		ID3D11DeviceContext* context;

	public:
		DX11VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, Vertex vertices[], size_t size);
		~DX11VertexBuffer();

		virtual void set();
	};

	DX11Factory(VertexBuffer);

}
