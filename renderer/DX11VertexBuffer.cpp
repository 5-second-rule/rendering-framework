#include "DX11VertexBuffer.h"


DX11VertexBuffer::DX11VertexBuffer(Vertex vertices[], size_t num, ID3D11Device* device, ID3D11DeviceContext* context) : VertexBuffer(num), context(context) {
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = (sizeof Vertex) * num;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&bd, NULL, &this->buffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(this->buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, num);
	context->Unmap(this->buffer, NULL);
}


DX11VertexBuffer::~DX11VertexBuffer()
{
	this->buffer->Release();
}

void DX11VertexBuffer::set() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &this->buffer, &stride, &offset);
}