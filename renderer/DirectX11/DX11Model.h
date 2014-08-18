#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <vector>

#include "Model.h"

#include "DX11Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Transmission {

	class DX11Model : public Model
	{
	protected:
		ID3D11DeviceContext* context;
		Renderer* renderer;

	public:
		DX11Model(ID3D11Device* device, ID3D11DeviceContext* context, VertexBuffer* vertices, IndexBuffer* indices, std::vector<Texture*> textures);
		virtual ~DX11Model();

		virtual void draw(Shader& vertexShader, Shader& pixelShader);
	};

	DX11Factory(Model);
}
