#pragma once


#include "render-framework.h"
#include "Window.h"

namespace Transmission {

	class FRAMEWORKDLL Sound {

	public:

		static Sound* Sound::createSound( Window* window );

		int LoadSoundFile( char* file );
		virtual bool PlaySoundAt( size_t index, bool loop ) = 0;
		virtual bool StopSoundAt( size_t index ) = 0;

	private:
		virtual int LoadWaveFile( char* file ) = 0;
	};
}