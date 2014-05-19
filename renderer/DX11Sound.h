#pragma once
#include <vector>
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include "Sound.h"


#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

namespace Transmission {
	class DX11Sound :
		public Sound {
	protected:

		struct WaveHeaderType {
			char chunkId[4];
			unsigned long chunkSize;
			char format[4];
			char subChunkId[4];
			unsigned long subChunkSize;
			unsigned short audioFormat;
			unsigned short numChannels;
			unsigned long sampleRate;
			unsigned long bytesPerSecond;
			unsigned short blockAlign;
			unsigned short bitsPerSample;
			char dataChunkId[4];
			unsigned long dataSize;
		};

		bool InitializeDirectSound( HWND );
		void ShutdownDirectSound();

		int LoadWaveFile( char* file );

		bool PlaySoundAt( int index, bool loop );

		IDirectSound8* m_DirectSound;
		IDirectSoundBuffer* m_primaryBuffer;
		std::vector<IDirectSoundBuffer8*>* secondaryBuffers;

	public:
		DX11Sound( HWND hwnd );
		~DX11Sound();
	};
}
