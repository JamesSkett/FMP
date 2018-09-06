#include "Time.h"



Time::Time()
{
}


Time::~Time()
{
}

void Time::StartTimer()
{
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);

	countsPerSecond = double(frequencyCount.QuadPart);

	QueryPerformanceCounter(&frequencyCount);
	CounterStart = frequencyCount.QuadPart;
}

double Time::GetTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return double(currentTime.QuadPart - CounterStart) / countsPerSecond;
}

int Time::GetFPS()
{
	frameCount++;

	if (GetTime() > 1.0f)
	{
		fps = frameCount;
		frameCount = 0;
		StartTimer();
	}

	return fps;
}
