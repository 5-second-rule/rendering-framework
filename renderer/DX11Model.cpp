#include "DX11Model.h"


DX11Model::DX11Model(VertexBuffer* v, IndexBuffer* i, ID3D11DeviceContext* context) : Model(v, i) {
	this->context = context;
}

DX11Model::~DX11Model()
{
}

void DX11Model::draw() {
	vertexBuffer->set();
	indexBuffer->set();

	context->DrawIndexed(indexBuffer->count(), 0, 0);
	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//context->Draw(3, 0);
}
