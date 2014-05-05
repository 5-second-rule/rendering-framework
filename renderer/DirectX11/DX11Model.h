#pragma once

#include <d3d11.h>
#include <vector>

#pragma comment(lib, "d3d11.lib")

#include "Model.h"

#include "DX11VertexBuffer.h"
#include "DX11IndexBuffer.h"
#include "Renderer.h"

namespace Transmission {

	class DX11Model : public Model
	{
	private:
		ID3D11DeviceContext* context;
		Renderer* renderer;

	public:
		DX11Model(VertexBuffer*, IndexBuffer*, ID3D11DeviceContext*, Texture*, Renderer*);
		DX11Model(VertexBuffer*, IndexBuffer*, ID3D11DeviceContext*, Texture*, Renderer*, Shader* vs, Shader* ps);
		~DX11Model();

		virtual void setShaders(Shader*, Shader*);
		virtual void setVertexShader(Shader*);
		virtual void setPixelShader(Shader*);
		virtual void draw();
	};
}
