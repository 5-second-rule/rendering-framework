#include "DX11Model.h"
#include "fbxsdk.h"



DX11Model::DX11Model(VertexBuffer* v, IndexBuffer* i, ID3D11DeviceContext* context) : Model(v, i) {
	this->context = context;
	vertices = new std::vector<Vertex>();
	indices = new std::vector<int>();
}

DX11Model::~DX11Model()
{
	delete(vertices);
}


void DX11Model::draw() {
	vertexBuffer->set();
	indexBuffer->set();

	context->DrawIndexed(indexBuffer->count(), 0, 0);
}