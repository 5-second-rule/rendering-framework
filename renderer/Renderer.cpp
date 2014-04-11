#include "Renderer.h"


// Renderer Choice
#if RENDERER == RENDERER_DX
	#include "DirectX11\DX11Renderer.h"
#elif RENDERER == RENDERER_GL
	#include "OpenGL\GLRenderer.h"
#endif

Renderer* Renderer::createRenderer(Window* window) {

#if RENDERER == RENDERER_DX
	return new DX11Renderer(window);
#elif RENDERER == RENDERER_GL
	return new GLRenderer(window);
#else
	#error "Unsupported Renderer"
#endif
}

