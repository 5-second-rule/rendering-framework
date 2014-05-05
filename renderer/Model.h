#pragma once
#include "util.h"

#include "Transformer.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Shader.h"

namespace Transmission {


	class FRAMEWORKDLL Model : public Transformer
	{
	protected:
		VertexBuffer* vertexBuffer;
		IndexBuffer* indexBuffer;
		Texture* texture;
		Shader* vertexShader;
		Shader* pixelShader;

	public:
		Model(VertexBuffer*, IndexBuffer*);
		~Model();

		virtual void setShaders(Shader*, Shader*) = 0;
		virtual void setVertexShader(Shader*) = 0;
		virtual void setPixelShader(Shader*) = 0;
		virtual void draw() = 0;
	};
}
