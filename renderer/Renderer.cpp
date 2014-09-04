#include "Renderer.h"

#include "common/Timer.h"

#include <iostream>


#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

// Renderer Choice
#if RENDERER == RENDERER_DX
	#include "DirectX11\DX11Renderer.h"
#elif RENDERER == RENDERER_GL
	#include "OpenGL\GLRenderer.h"
#endif

namespace Transmission {

	Renderer* Renderer::createRenderer(Window* window) {
		PROFILE("Renderer::createRenderer");

		Renderer* r;

		#if RENDERER == RENDERER_DX
			r = new DX11Renderer(window);
		#elif RENDERER == RENDERER_GL
			r = new GLRenderer(window, vertex, pixel);
		#else
			#error "Unsupported Renderer"
		#endif

		return window->renderer = r;
	}
}
