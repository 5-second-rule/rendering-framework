#pragma once
#include "render-framework.h"
#include "util.h"

#include "Window.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Model.h"
#include "Camera.h"

class FRAMEWORKDLL Renderer
{
public:
	static Renderer* createRenderer(Window*);

	/* ----- */

	virtual void clearFrame() = 0;
	virtual void drawFrame() = 0;

	virtual VertexBuffer* createVertexBuffer(Vertex[], size_t) = 0;
	virtual IndexBuffer* createIndexBuffer(unsigned int[], size_t) = 0;
	virtual Model* createModel(VertexBuffer*, IndexBuffer*) = 0;

	virtual Camera* getCamera() = 0;

	virtual void setObjectMatrix(ITransformable*) = 0;
};

