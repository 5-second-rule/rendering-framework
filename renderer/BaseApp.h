#pragma once

#include "Window.h"
#include "Renderer.h"

class BaseApp
{
protected:
	Window& window;
	Renderer& renderer;

	virtual bool run();
public:
	BaseApp(Window&, Renderer&);
	~BaseApp();

	void start();
};

