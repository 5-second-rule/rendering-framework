#include "DX11Model.h"
#include "fbxsdk.h"

namespace Transmission {


	DX11Model::DX11Model(VertexBuffer* v, IndexBuffer* i, ID3D11DeviceContext* context, Texture* texture, Renderer* renderer) : Model(v, i) {
		this->context = context;
		this->texture = texture;
		this->renderer = renderer;
	}

	DX11Model::~DX11Model() {}


	void DX11Model::draw() {
		vertexBuffer->set();
		indexBuffer->set();
		texture->set();
		this->renderer->setObjectMatrix(this);
		context->DrawIndexed(indexBuffer->count(), 0, 0);
	}

}
