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
	public:
		DX11VertexBuffer(DX11Renderer* renderer, Vertex vertices[], size_t size);
		~DX11VertexBuffer();

		virtual void set();

	private:
		ID3D11Buffer* buffer;
		ID3D11DeviceContext* context;
	};

	DX11Factory(VertexBuffer);

}
