#include "Sound.h"

// Sound Engine Choice
#if RENDERER == RENDERER_DX
#include "DX11Sound.h"
#elif RENDERER == RENDERER_GL
#include "OpenAL\ALRenderer.h"
#endif

namespace Transmission {

	Sound* Sound::createSound( Window* window ) {

#if RENDERER == RENDERER_DX
		return new DX11Sound( (HWND)window->getHandle() );
#elif RENDERER == RENDERER_GL
		return new GLRenderer( window );
#else
#error "Unsupported Renderer"
#endif
	}

	int Sound::LoadSoundFile(char* file) {
		char* extension;

		extension = strrchr( file, '.' );

		// Check extension and used necessary sound file loader
		if( strcmp( extension, ".wav" ) == 0 ) {
			return this->LoadWaveFile( file );
		} else {
			return -1;
		}
	}

}