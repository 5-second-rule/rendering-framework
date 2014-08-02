#pragma once

#include <d3d11.h>
#include <vector>

#pragma comment(lib, "d3d11.lib")

#include "Model.h"

#include "Renderer.h"
#include "DX11Renderer.h"

class VertexBuffer;
class IndexBuffer;

namespace Transmission {

	class DX11Model : public Model
	{
	protected:
		ID3D11DeviceContext* context;
		Renderer* renderer;

	public:
		DX11Model(VertexBuffer* v, IndexBuffer* i, ID3D11DeviceContext* context, Texture* texture, Renderer* renderer);
		DX11Model(VertexBuffer* v, IndexBuffer* i, ID3D11DeviceContext* context, Texture* texture, Texture* bump, Renderer* renderer);
		DX11Model(VertexBuffer* v, IndexBuffer* i, ID3D11DeviceContext* context, Texture* texture, Renderer* renderer, Shader* vs, Shader* ps);
		DX11Model(VertexBuffer* v, IndexBuffer* i, ID3D11DeviceContext* context, Texture* texture, Texture* bumpMap, Renderer*, Shader* vs, Shader* ps);
		virtual ~DX11Model();

		virtual void setShaders(Shader*, Shader*);
		virtual void setVertexShader(Shader*);
		virtual void setPixelShader(Shader*);
		virtual void draw();
	};

	DX11Factory(Model);
}
