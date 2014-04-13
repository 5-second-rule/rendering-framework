#include "DX11VertexBuffer.h"

#include <stdexcept>

DX11VertexBuffer::DX11VertexBuffer(Vertex vertices[], size_t num, ID3D11Device* device, ID3D11DeviceContext* context) : VertexBuffer(num), context(context) {
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = (sizeof Vertex) * num;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;

	if (FAILED(device->CreateBuffer(&bd, &initData, &this->buffer)))
		throw std::runtime_error("Couldn't create Vertex Buffer");

}


DX11VertexBuffer::~DX11VertexBuffer()
{
	this->buffer->Release();
}

void DX11VertexBuffer::set() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &this->buffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}