#include "DX11Sound.h"

namespace Transmission {

	DX11Sound::DX11Sound( IDirectSoundBuffer8* buffer ) : secondaryBuffer( buffer ) {}

	DX11Sound::~DX11Sound() {
		this->secondaryBuffer->Release();
	}

	bool DX11Sound::play() {
		HRESULT result;

		// Set position at the beginning of the sound buffer.
		result = this->secondaryBuffer->SetCurrentPosition( 0 );
		if( FAILED( result ) ) {
			return false;
		}

		// Play the contents of the secondary sound buffer.
		result = this->secondaryBuffer->Play( 0, 0, 0 );
		if( FAILED( result ) ) {
			return false;
		}

		return true;
	}

	bool DX11Sound::playLooped() {
		HRESULT result;

		// Set position at the beginning of the sound buffer.
		result = this->secondaryBuffer->SetCurrentPosition( 0 );
		if( FAILED( result ) ) {
			return false;
		}

		// Set volume of the buffer to 100%.
		result = this->secondaryBuffer->SetVolume( DSBVOLUME_MAX );
		if( FAILED( result ) ) {
			return false;
		}

		// Play the contents of the secondary sound buffer.
		result = this->secondaryBuffer->Play( 0, 0, DSBPLAY_LOOPING );
		if( FAILED( result ) ) {
			return false;
		}

		return true;
	}

	bool DX11Sound::stop() {

		HRESULT result;

		// stop the contents of the secondary sound buffer.
		result = this->secondaryBuffer->Stop();

		if( FAILED( result ) ) {
			return false;
		}

		return true;
	}

	bool DX11Sound::setVolume(int attenuation) {
		// max attentuation of 100 dB allowed, basically silent
		if( attenuation > 10000 ) attenuation = 10000;
		// no amplification allowed, force to 0 (DSBVOLUME_MAX)
		if( attenuation < 0 ) attenuation = 0;

		HRESULT result = this->secondaryBuffer->SetVolume( attenuation );
		if( FAILED( result ) ) {
			return false;
		}

		return true;
	}
}