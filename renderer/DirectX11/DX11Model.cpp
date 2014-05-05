#include "DX11Model.h"
#include "fbxsdk.h"

namespace Transmission {


	DX11Model::DX11Model(VertexBuffer* v, IndexBuffer* i, ID3D11DeviceContext* context, Texture* texture, Renderer* renderer) : Model(v, i) {
		this->context = context;
		this->texture = texture;
		this->renderer = renderer;
		this->vertexShader = renderer->getDefaultVertexShader();
		this->pixelShader = renderer->getDefaultPixelShader();
	}

	DX11Model::DX11Model(VertexBuffer* v, IndexBuffer* i, ID3D11DeviceContext* context, Texture* texture, Renderer* renderer, Shader* vertexShader, Shader* pixelShader) : Model(v, i) {
		this->context = context;
		this->texture = texture;
		this->renderer = renderer;
		this->vertexShader = vertexShader;
		this->pixelShader = pixelShader;
	}

	DX11Model::~DX11Model() {}

	void DX11Model::setShaders(Shader* vertexShader, Shader* pixelShader){
		this->vertexShader = vertexShader;
		this->pixelShader = pixelShader;
	}

	void DX11Model::setVertexShader(Shader* vertexShader){
		this->vertexShader = vertexShader;
	}

	void DX11Model::setPixelShader(Shader* pixelShader){
		this->pixelShader = pixelShader;
	}

	void DX11Model::draw() {
		vertexBuffer->set();
		indexBuffer->set();
		vertexShader->setWithNoLayout();
		pixelShader->set();
		texture->set();
		this->renderer->setObjectMatrix(this->getTransform());
		context->DrawIndexed(indexBuffer->count(), 0, 0);
	}

}
