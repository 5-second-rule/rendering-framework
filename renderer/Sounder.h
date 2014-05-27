#pragma once


#include "render-framework.h"
#include "Sound.h"
#include "Window.h"

namespace Transmission {

	class FRAMEWORKDLL Sounder {

	public:
		static Sounder* Sounder::createSounder( Window* window );
		Sound* createSoundFromFile( char* file );
	private:
		virtual Sound* LoadWaveFile( char* file ) = 0;
	};
}