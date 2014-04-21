#pragma once
#include "render-framework.h"


class FRAMEWORKDLL Window
{
public:

	static const unsigned int screenWidth = 800;
	static const unsigned int screenHeight = 600;
	static Window* createWindow(void* handle);

	// -----

	virtual void* getHandle() = 0;
	virtual void* getInput() = 0;
	enum MessageType { None = 0, Message, Quit };
	virtual MessageType getMessage() = 0;
};

