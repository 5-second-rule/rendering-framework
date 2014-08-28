#pragma once
#include "render-framework.h"

#include "Input.h"

namespace Transmission {

	class Renderer;

	class FRAMEWORKDLL Window
	{
	protected:

		unsigned int width;
		unsigned int height;
		const wchar_t* name;

		Window(const wchar_t* name, unsigned int width, unsigned int height);

	public:

		static Window* createWindow(void* handle, const wchar_t* name, unsigned int width, unsigned int height);

		// this probably shouldn't be public
		Renderer* renderer;

		// -----

		virtual void* getHandle() = 0;
		virtual const Input* getInput() = 0;
		enum MessageType { None = 0, Message, Quit };
		virtual MessageType getMessage() = 0;

		const wchar_t* getName();
		unsigned int getWidth();
		unsigned int getHeight();
		void setWidth(unsigned int width);
		void setHeight(unsigned int height);
	};

}
