#pragma once


#include "render-framework.h"
#include "Window.h"

namespace Transmission {

	class FRAMEWORKDLL Sound {

	public:

		static Sound* Sound::createSound( Window* window );

		int LoadSoundFile( char* file );
		virtual bool PlaySoundAt( int index, bool loop ) = 0;
		virtual bool StopSoundAt( int index ) = 0;

	private:
		virtual int LoadWaveFile( char* file ) = 0;
	};
}