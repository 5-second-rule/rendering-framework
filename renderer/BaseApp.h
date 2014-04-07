#pragma once
#include "render-framework.h"

#include "Window.h"
#include "Renderer.h"

class FRAMEWORKDLL BaseApp
{
protected:
	Window* window;
	Renderer* renderer;

	virtual bool run();
public:
	BaseApp(void* handle);
	~BaseApp();

	void start();
};

