#pragma once

#include "Sound.h"

#include <dsound.h>


namespace Transmission {

	class DX11Sound : public Sound {
	private:
		IDirectSoundBuffer8* secondaryBuffer;
	public:
		DX11Sound( IDirectSoundBuffer8* buffer );
		virtual ~DX11Sound();

		bool play();
		bool playLooped();
		bool stop();

		virtual bool setVolume( int attenuation );
	};
}