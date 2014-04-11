#pragma once
#include "util.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class FRAMEWORKDLL Model
{
protected:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

public:
	Model(VertexBuffer*, IndexBuffer*);
	~Model();

	virtual void draw() = 0;
};

