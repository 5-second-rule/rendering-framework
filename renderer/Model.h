#pragma once
#include "util.h"

#include <vector>

#include "Transformer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

namespace Transmission {


	class FRAMEWORKDLL Model : public Transformer
	{
	protected:
		VertexBuffer* vertexBuffer;
		IndexBuffer* indexBuffer;

		std::vector<Texture*> textures;

	public:
		Model(VertexBuffer* vertices, IndexBuffer* indices, std::initializer_list<Texture*> textures)
			: vertexBuffer(vertices), indexBuffer(indices), textures(textures) {}
		virtual ~Model() = default;

		virtual void draw(Shader& vertexShader, Shader& pixelShader) = 0;
	};
}
