#pragma once
#include "render-framework.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	virtual void clearFrame() = 0;
	virtual void drawFrame() = 0;
};

