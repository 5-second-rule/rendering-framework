#include "Window.h"

// Platform Choice
#ifdef _WIN32
#include "Win32\Win32Window.h"
#elif __APPLE__
#include "OSX\OSXWindow.h"
#endif

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

namespace Transmission {

	Window::Window(const wchar_t* name, unsigned int width, unsigned int height)
		: name(name)
		, width(width)
		, height(height)
	{}

	Window* Window::createWindow(void* handle, const wchar_t* name, unsigned int width, unsigned int height) {

#ifdef _WIN32
		return new Win32Window((HINSTANCE)handle, name, width, height);
#elif __APPLE__
		return new OSXWindow((NSWindow*) handle);
#else
#error "Unsupported Platform"
#endif

	}

	const wchar_t* Window::getName() {
		return this->name;
	}

	unsigned int Window::getWidth() {
		return this->width;
	}

	unsigned int Window::getHeight() {
		return this->height;
	}

	void Window::setWidth(unsigned int width) {
		this->width = width;
	}

	void Window::setHeight(unsigned int height) {
		this->height = height;
	}

}
