#pragma once
#include "render-framework.h"

#include "Input.h"

namespace Transmission {

	class FRAMEWORKDLL Window
	{
	protected:
		unsigned int width;
		unsigned int height;
		const wchar_t* name;

		Window(const wchar_t* name, unsigned int width, unsigned int height);

	public:

		static Window* createWindow(void* handle, const wchar_t* name, unsigned int width, unsigned int height);

		// -----

		virtual void* getHandle() = 0;
		virtual const Input* getInput() = NULL;
		enum MessageType { None = 0, Message, Quit };
		virtual MessageType getMessage() = 0;

		const wchar_t* getName();
		unsigned int getWidth();
		unsigned int getHeight();
	};

}
