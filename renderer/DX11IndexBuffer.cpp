#include "DX11IndexBuffer.h"


DX11IndexBuffer::DX11IndexBuffer(unsigned int indices[], size_t num, ID3D11Device* device, ID3D11DeviceContext* context) : IndexBuffer(num), context(context) {
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = (sizeof (unsigned int)) * num;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&bd, NULL, &this->buffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(this->buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, indices, num);
	context->Unmap(this->buffer, NULL);
}


DX11IndexBuffer::~DX11IndexBuffer()
{
	this->buffer->Release();
}

void DX11IndexBuffer::set() {
	context->IASetIndexBuffer(this->buffer, DXGI_FORMAT_R32_UINT, 0);
}
