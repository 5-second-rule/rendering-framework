#pragma once
#include "util.h"

#include "Transformer.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Texture.h"

namespace Transmission {


	class FRAMEWORKDLL Model : public Transformer
	{
	protected:
		VertexBuffer* vertexBuffer;
		IndexBuffer* indexBuffer;
		Texture* texture;

	public:
		Model(VertexBuffer*, IndexBuffer*);
		~Model();

		virtual void draw() = 0;
	};
}
