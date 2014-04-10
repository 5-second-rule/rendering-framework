#include "BaseApp.h"

#include "render-framework.h"

#include <chrono>
#include <thread>

// Platform Choice
#ifdef _WIN32
#include "Win32\Win32Window.h"
#elif __APPLE__
#include "OSX\OSXWindow.h"
#endif

// Renderer Choice
#if RENDERER == RENDERER_DX
#include "DirectX11\DX11Renderer.h"
#elif RENDERER == RENDERER_GL
#include "OpenGL\GLRenderer.h"
#endif

VertexBuffer* vbuf;
IndexBuffer* ibuf;
Model* model;

BaseApp::BaseApp(void* handle) :
#ifdef _WIN32
	window(new Win32Window((HINSTANCE)handle)),
#elif __APPLE__
	window(new OSXWindow((NSWindow)handle)),
#else
	#error "Unsupported Platform"
#endif

#if RENDERER == RENDERER_DX
	renderer(new DX11Renderer(*this->window))
#elif RENDERER == RENDERER_GL
	renderer(new GLRenderer(*this->window))
#else
	#error "Unsupported Renderer"
#endif

{
		Vertex triangle[3] = {
			{ XMFLOAT3(0, 0, 1), XMFLOAT4(1, 0, 0, 1) },
			{ XMFLOAT3(0, 1, 1), XMFLOAT4(0, 1, 0, 1) },
			{ XMFLOAT3(1, 0, 1), XMFLOAT4(0, 0, 1, 1) }
		};

		UINT index[3] = { 0, 1, 2 };

		vbuf = renderer->createVertexBuffer(triangle, 3);
		ibuf = renderer->createIndexBuffer(index, 3);
		model = renderer->createModel(vbuf, ibuf);
}


BaseApp::~BaseApp()
{

	delete model;
	delete vbuf;
	delete ibuf;

	delete this->renderer;
	delete this->window;

	
}

bool BaseApp::run() {
	
	// Pull all messages
	Window::MessageType t;
	while ((t = window->getMessage()) != Window::MessageType::None) {
		if (t == Window::MessageType::Quit)
			return false;
	}

	renderer->clearFrame();

	model->draw();

	renderer->drawFrame();

	return true;
}

void BaseApp::start() {
	while (run()) std::this_thread::sleep_for(std::chrono::milliseconds(500));;
}
