#include "DX11IndexBuffer.h"

#include "dxerr.h"

namespace Transmission {

	DX11IndexBuffer::DX11IndexBuffer(DX11Renderer* renderer, unsigned int indices[], size_t num)
		: IndexBuffer(num), context(renderer->getContext())
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = (sizeof(unsigned int)) * num;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof initData);
		initData.pSysMem = indices;

		HR(renderer->getDevice()->CreateBuffer(&bd, &initData, &this->buffer));
	}


	DX11IndexBuffer::~DX11IndexBuffer()
	{
		this->buffer->Release();
	}

	void DX11IndexBuffer::set() {
		context->IASetIndexBuffer(this->buffer, DXGI_FORMAT_R32_UINT, 0);
	}

}