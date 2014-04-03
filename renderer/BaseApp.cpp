#include "BaseApp.h"


BaseApp::BaseApp(Window& window, Renderer& renderer)
	: window(window)
	, renderer(renderer)
{
	//TODO
}


BaseApp::~BaseApp()
{
}

bool BaseApp::run() {
	
	// Pull all messages
	Window::MessageType t;
	while ((t = window.getMessage()) != Window::MessageType::None) {
		if (t == Window::MessageType::Quit) return false;
	}

	renderer.renderFrame();

	return true;
}

void BaseApp::start() {
	while(run());
}
