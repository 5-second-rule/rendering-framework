#pragma once
#include "render-framework.h"

class FRAMEWORKDLL Window
{
public:

	static Window* createWindow(void* handle);

	// -----

	virtual void* getHandle() = 0;

	enum MessageType { None = 0, Message, Quit };
	virtual MessageType getMessage() = 0;
};

