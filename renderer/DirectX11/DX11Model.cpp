#include "DX11Model.h"

namespace Transmission {


	DX11Model::DX11Model( DX11Renderer* renderer,
		VertexBuffer* vertices, IndexBuffer* indices, std::initializer_list<Texture*> textures
	) : Model(vertices, indices, textures), renderer(renderer) {}

	DX11Model::~DX11Model() {}

	void DX11Model::draw(Shader& vertexShader, Shader& pixelShader) {
		this->vertexBuffer->set();
		this->indexBuffer->set();
		
		vertexShader.set();
		pixelShader.set();
		
		for (unsigned int i = 0; i < this->textures.size(); ++i) {
			this->textures[i]->set(i);
		}

		this->renderer->setObjectMatrix(this->getTransform());
		this->renderer->getContext()->DrawIndexed(indexBuffer->count(), 0, 0);
	}

}
