#pragma once

#include <d3d11.h>
#include <vector>

#pragma comment(lib, "d3d11.lib")

#include "DX11Model.h"

#include "DX11VertexBuffer.h"
#include "DX11IndexBuffer.h"
#include "Renderer.h"

namespace Transmission {

	class DX11Model2D : public DX11Model
	{
	private:
		bool buffersMade;
	public:
		DX11Model2D(VertexBuffer* v, IndexBuffer* i, ID3D11DeviceContext* context, Texture* texture, Renderer* renderer);
		DX11Model2D(VertexBuffer* v, IndexBuffer* i, ID3D11DeviceContext* context, Texture* texture, Renderer* renderer, Shader* vs, Shader* ps);
		DX11Model2D(Vertex* v, int numVertices, Index* i, int numIndices, ID3D11DeviceContext* context, Texture* texture, Renderer* renderer);
		DX11Model2D(Vertex* v, int numVertices, Index* i, int numIndices, ID3D11DeviceContext* context, Texture* texture, Renderer* renderer, Shader* vs, Shader* ps);
		~DX11Model2D();

		virtual void setShaders(Shader*, Shader*);
		virtual void setVertexShader(Shader*);
		virtual void setPixelShader(Shader*);
		virtual void draw();
	};
}
