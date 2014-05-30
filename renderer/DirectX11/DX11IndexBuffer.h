#pragma once
#include "IndexBuffer.h"

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include "util.h"

namespace Transmission {
	class DX11IndexBuffer :
		public IndexBuffer
	{
	private:
		ID3D11Buffer* buffer;
		ID3D11DeviceContext* context;

	public:
		DX11IndexBuffer(unsigned int [], size_t, ID3D11Device*, ID3D11DeviceContext*);
		virtual ~DX11IndexBuffer();

		virtual void set();
	};
}