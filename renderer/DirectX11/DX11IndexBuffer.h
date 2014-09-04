#pragma once
#include "IndexBuffer.h"

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include "util.h"

#include "Renderer.h"
#include "DX11Renderer.h"

namespace Transmission {
	class FRAMEWORKDLL DX11IndexBuffer :
		public IndexBuffer
	{
	private:
		ID3D11Buffer* buffer;
		ID3D11DeviceContext* context;

	public:
		DX11IndexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, unsigned int* indices, size_t size);
		virtual ~DX11IndexBuffer();

		virtual void set();
	};

	DX11Factory(IndexBuffer);
}