#include "DX11IndexBuffer.h"

#include <stdexcept>

DX11IndexBuffer::DX11IndexBuffer(unsigned int indices[], size_t num, ID3D11Device* device, ID3D11DeviceContext* context) : IndexBuffer(num), context(context) {
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = (sizeof (unsigned int)) * num;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof initData);
	initData.pSysMem = indices;

	if (FAILED(device->CreateBuffer(&bd, &initData, &this->buffer)))
		throw std::runtime_error("Couldn't create Index Buffer");

}


DX11IndexBuffer::~DX11IndexBuffer()
{
	this->buffer->Release();
}

void DX11IndexBuffer::set() {
	context->IASetIndexBuffer(this->buffer, DXGI_FORMAT_R32_UINT, 0);
}
