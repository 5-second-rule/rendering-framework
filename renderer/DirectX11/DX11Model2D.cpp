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

		buffersMade = false;
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

		buffersMade = false;
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

		vertexBuffer = renderer->createVertexBuffer(v, numVertices);
		indexBuffer = renderer->createIndexBuffer(i, numIndices);
		buffersMade = true;
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

		vertexBuffer = renderer->createVertexBuffer(v, numVertices);
		indexBuffer = renderer->createIndexBuffer(i, numIndices);
		buffersMade = true;
	}

	DX11Model2D::~DX11Model2D() {
		if (buffersMade) {
			delete vertexBuffer;
			delete indexBuffer;
			vertexBuffer = NULL;
			indexBuffer = NULL;
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
		DX11Model::draw();
		this->renderer->end2D();
	}
}
