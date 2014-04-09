#pragma once
#include "render-framework.h"
#include "util.h"

#include "VertexBuffer.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	virtual void clearFrame() = 0;
	virtual void drawFrame() = 0;

	virtual VertexBuffer* createVertexBuffer(Vertex[], size_t) = 0;
};

