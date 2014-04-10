#include "BaseApp.h"

#include "render-framework.h"

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
	//TODO
}


BaseApp::~BaseApp()
{
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

	Vertex triangle[3] = {
		{ 0, 0, 0, 1, 0, 0, 1 },
		{ 1, 0, 0, 0, 1, 0, 1 },
		{ 0, 1, 0, 0, 0, 1, 1 }
	};

	UINT index[3] = { 0, 1, 2 };

	VertexBuffer* vbuf = renderer->createVertexBuffer(triangle, 3);
	IndexBuffer* ibuf = renderer->createIndexBuffer(index, 3);
	//Model* model = renderer->createModel(vbuf, ibuf);

	//model->draw();

	renderer->drawFrame();


	//delete model;
	delete vbuf;
	delete ibuf;

	return true;
}

void BaseApp::start() {
	while(run());
}
