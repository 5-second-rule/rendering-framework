#pragma once
#include "render-framework.h"

class Window
{
public:
	Window();
	~Window();

	virtual void* getHandle() = 0;

	enum MessageType { None = 0, Message, Quit };
	virtual MessageType getMessage() = 0;
};

