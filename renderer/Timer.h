#pragma once
#include "render-framework.h"

namespace Transmission {

	class FRAMEWORKDLL Timer
	{
	protected:
		float timeSinceLastFrame;

	public:
		Timer();
		~Timer();

		virtual void StartTimer() = 0;
		virtual void ResetFPSTimer() = 0;
		virtual float GetTime() = 0;
		virtual float GetFPSTime() = 0;
		virtual float GetFrameTime() = 0;
		virtual void GetElapsedTimeAndTimeSinceLastFrame(float*, float*) = 0;
		virtual float GetCalculatedTimeSinceLastFrame() = 0;
	};
}
