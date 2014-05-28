#include "DX11Model2D.h"
#include "fbxsdk.h"

namespace Transmission {

	DX11Model2D::DX11Model2D(
		VertexBuffer* v, 
		IndexBuffer* i, 
		ID3D11DeviceContext* context, 
		Texture* texture, 
		Renderer* renderer) : 
		DX11Model(v, i, context, texture, renderer) {

		this->buffersMade = false;
		this->isTransparent = false;
	}

	DX11Model2D::DX11Model2D(
		VertexBuffer* v, 
		IndexBuffer* i, 
		ID3D11DeviceContext* context, 
		Texture* texture, 
		Renderer* renderer, 
		Shader* vertexShader, 
		Shader* pixelShader) : 
		DX11Model(v, i, context, texture, renderer, vertexShader, pixelShader) {

		this->buffersMade = false;
		this->isTransparent = false;
	}

	DX11Model2D::DX11Model2D(
		Vertex* v,
		int numVertices,
		Index* i,
		int numIndices,
		ID3D11DeviceContext* context,
		Texture* texture,
		Renderer* renderer) :
		DX11Model(nullptr, nullptr, context, texture, renderer) {

		this->vertexBuffer = renderer->createVertexBuffer(v, numVertices);
		this->indexBuffer = renderer->createIndexBuffer(i, numIndices);
		this->buffersMade = true;
		this->isTransparent = false;
	}

	DX11Model2D::DX11Model2D(
		Vertex* v,
		int numVertices,
		Index* i,
		int numIndices,
		ID3D11DeviceContext* context,
		Texture* texture,
		Renderer* renderer,
		Shader* vertexShader,
		Shader* pixelShader) : 
		DX11Model(nullptr, nullptr, context, texture, renderer, vertexShader, pixelShader) {

		this->vertexBuffer = renderer->createVertexBuffer(v, numVertices);
		this->indexBuffer = renderer->createIndexBuffer(i, numIndices);
		this->buffersMade = true;
		this->isTransparent = false;
	}

	DX11Model2D::DX11Model2D(
		Vertex* v,
		int numVertices,
		Index* i,
		int numIndices,
		ID3D11DeviceContext* context,
		Texture* texture,
		Renderer* renderer,
		bool isTransparent) :
		DX11Model(nullptr, nullptr, context, texture, renderer) {

		this->vertexBuffer = renderer->createVertexBuffer(v, numVertices);
		this->indexBuffer = renderer->createIndexBuffer(i, numIndices);
		this->buffersMade = true;
		this->isTransparent = isTransparent;
	}

	DX11Model2D::DX11Model2D(
		Vertex* v,
		int numVertices,
		Index* i,
		int numIndices,
		ID3D11DeviceContext* context,
		Texture* texture,
		Renderer* renderer,
		Shader* vertexShader,
		Shader* pixelShader,
		bool isTransparent) :
		DX11Model(nullptr, nullptr, context, texture, renderer, vertexShader, pixelShader) {

		this->vertexBuffer = renderer->createVertexBuffer(v, numVertices);
		this->indexBuffer = renderer->createIndexBuffer(i, numIndices);
		this->buffersMade = true;
		this->isTransparent = isTransparent;
	}

	DX11Model2D::~DX11Model2D() {
		if (this->buffersMade) {
			delete this->vertexBuffer;
			delete this->indexBuffer;
			this->vertexBuffer = NULL;
			this->indexBuffer = NULL;
		}
	}

	void DX11Model2D::setShaders(Shader* vertexShader, Shader* pixelShader) {
		DX11Model::setShaders(vertexShader, pixelShader);
	}

	void DX11Model2D::setVertexShader(Shader* vertexShader) {
		DX11Model::setVertexShader(vertexShader);
	}

	void DX11Model2D::setPixelShader(Shader* pixelShader) {
		DX11Model::setPixelShader(pixelShader);
	}

	void DX11Model2D::draw() {
		this->renderer->prep2D();
		if (this->isTransparent) {
			this->renderer->makeTransparent();
			DX11Model::draw();
			this->renderer->makeOpaque();
		} else {
			DX11Model::draw();
		}
		this->renderer->end2D();
	}
}
