#include "Sounder.h"

// Sound Engine Choice
#if RENDERER == RENDERER_DX
#include "DX11Sounder.h"
#elif RENDERER == RENDERER_GL
#include "OpenAL\ALRenderer.h"
#endif

namespace Transmission {

	Sounder* Sounder::createSounder( Window* window ) {

#if RENDERER == RENDERER_DX
		return new DX11Sounder( (HWND)window->getHandle() );
#elif RENDERER == RENDERER_GL
		return new GLRenderer( window );
#else
#error "Unsupported Renderer"
#endif
	}

	Sound* Sounder::createSoundFromFile(char* file) {
		char* extension;

		extension = strrchr( file, '.' );

		// Check extension and used necessary sound file loader
		if( strcmp( extension, ".wav" ) == 0 ) {
			return this->LoadWaveFile( file );
		} else {
			return nullptr;
		}
	}

}