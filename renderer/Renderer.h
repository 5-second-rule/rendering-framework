#pragma once
class Renderer
{
public:
	Renderer();
	~Renderer();

	virtual void initializePipeline() = 0;

	virtual void renderFrame() = 0;
};

