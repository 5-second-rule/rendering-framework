#pragma once
#include <Windows.h>

#include "Timer.h"

namespace Transmission {

	class DX11Timer :
		public Timer
	{
	public:
		DX11Timer();
		~DX11Timer();

		void StartTimer();
		void ResetFPSTimer();
		float GetTime();
		float GetFPSTime();
		float GetFrameTime();
		void GetElapsedTimeAndTimeSinceLastFrame(float*, float*);
		float DX11Timer::GetCalculatedTimeSinceLastFrame();
	};

}