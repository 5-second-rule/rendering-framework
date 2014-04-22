#include "Window.h"

// Platform Choice
#ifdef _WIN32
#include "Win32\Win32Window.h"
#elif __APPLE__
#include "OSX\OSXWindow.h"
#endif

Window* Window::createWindow(void* handle) {

#ifdef _WIN32
	return new Win32Window((HINSTANCE)handle);
#elif __APPLE__
	return new OSXWindow((NSWindow*)handle);
#else
#error "Unsupported Platform"
#endif

}
