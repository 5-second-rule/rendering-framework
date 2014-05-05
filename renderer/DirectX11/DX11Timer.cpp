#include "DX11Timer.h"

namespace Transmission {

	DX11Timer::DX11Timer() : Timer()
	{
		timeSinceLastFrame = 0.0;
	}

	DX11Timer::~DX11Timer()
	{
	}

	float countsPerSecond = 0.0;
	__int64 CounterStart = 0;
	__int64 FPSCounterStart = 0;

	__int64 frameTimeOld = 0;

	void DX11Timer::StartTimer()
	{
		LARGE_INTEGER frequencyCount;
		QueryPerformanceFrequency(&frequencyCount);

		countsPerSecond = float(frequencyCount.QuadPart);

		QueryPerformanceCounter(&frequencyCount);
		CounterStart = frequencyCount.QuadPart;
		frameTimeOld = CounterStart;
		FPSCounterStart = CounterStart;
	}

	void DX11Timer::ResetFPSTimer()
	{
		LARGE_INTEGER frequencyCount;
		QueryPerformanceFrequency(&frequencyCount);

		countsPerSecond = float(frequencyCount.QuadPart);

		QueryPerformanceCounter(&frequencyCount);
		FPSCounterStart = frequencyCount.QuadPart;
	}

	float DX11Timer::GetTime()
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		return float(currentTime.QuadPart - CounterStart) / countsPerSecond;
	}

	float DX11Timer::GetFPSTime()
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		return float(currentTime.QuadPart - FPSCounterStart) / countsPerSecond;
	}

	float DX11Timer::GetFrameTime()
	{
		LARGE_INTEGER currentTime;
		__int64 tickCount;
		QueryPerformanceCounter(&currentTime);

		tickCount = currentTime.QuadPart - frameTimeOld;
		frameTimeOld = currentTime.QuadPart;

		if (tickCount < 0.0f)
			tickCount = 0.0f;

		return float(tickCount) / countsPerSecond;
	}

	void DX11Timer::GetElapsedTimeAndTimeSinceLastFrame(float* elapsedTime, float* timeLastFrame)
	{
		LARGE_INTEGER currentTime;
		__int64 tickCount;
		QueryPerformanceCounter(&currentTime);

		tickCount = currentTime.QuadPart - frameTimeOld;
		frameTimeOld = currentTime.QuadPart;

		if (tickCount < 0.0f)
			tickCount = 0.0f;

		*elapsedTime = float(currentTime.QuadPart - CounterStart) / countsPerSecond;
		timeSinceLastFrame = float(tickCount) / countsPerSecond;
		*timeLastFrame = timeSinceLastFrame;
	}

	float DX11Timer::GetCalculatedTimeSinceLastFrame()
	{
		return timeSinceLastFrame;
	}
}