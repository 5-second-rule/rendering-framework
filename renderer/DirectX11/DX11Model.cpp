#include "DX11Model.h"
#include "fbxsdk.h"

namespace Transmission {


	DX11Model::DX11Model(VertexBuffer* v, IndexBuffer* i, ID3D11DeviceContext* context, Renderer* renderer) : Model(v, i) {
		this->context = context;
		this->renderer = renderer;
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
		this->renderer->setObjectMatrix(this);
		context->DrawIndexed(indexBuffer->count(), 0, 0);
	}

}