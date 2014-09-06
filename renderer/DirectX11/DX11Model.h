#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <vector>

#include "Model.h"

#include "DX11Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Transmission {

	class FRAMEWORKDLL DX11Model : public Model
	{
	protected:
		DX11Renderer* renderer;

	public:
		DX11Model(DX11Renderer* renderer,
			VertexBuffer* vertices, IndexBuffer* indices,
			std::initializer_list<Texture*> textures
		);
		virtual ~DX11Model();

		virtual void draw(Shader& vertexShader, Shader& pixelShader);
	};

	DX11Factory(Model);
}
