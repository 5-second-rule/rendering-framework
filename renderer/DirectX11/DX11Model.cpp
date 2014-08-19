#include "DX11Model.h"

namespace Transmission {


	DX11Model::DX11Model(ID3D11Device* device, ID3D11DeviceContext* context, VertexBuffer* vertices, IndexBuffer* indices, std::initializer_list<Texture*> textures)
		: Model(vertices, indices, textures)
	{
		//TODO
	}

	DX11Model::~DX11Model() {}

	void DX11Model::draw(Shader& vertexShader, Shader& pixelShader) {
		this->vertexBuffer->set();
		this->indexBuffer->set();
		
		//vertexShader->setWithNoLayout();
		//pixelShader->set();
		
		for (unsigned int i = 0; i < this->textures.size(); ++i) {
			this->textures[i]->set(i);
		}

		this->renderer->setObjectMatrix(this->getTransform());
		context->DrawIndexed(indexBuffer->count(), 0, 0);
	}

}
