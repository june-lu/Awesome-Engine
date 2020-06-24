#include "Time.h"

int Time::fps = 0;
float Time::detalTime = 0;
float Time::allTime = 0;

Time::Time()
{
	frameCountInOneSecond = 0;
}


Time::~Time()
{
}

void Time::Start()
{
	lastFpsTime = startTime = SDL_GetTicks() / 1000.0;
}

void Time::Update()
{
	lastUpdateTime = allTime;
	allTime = SDL_GetTicks() / 1000.0 - startTime;
	detalTime = allTime - lastUpdateTime;
	frameCountInOneSecond++;
	if (allTime - lastFpsTime > 1)
	{
		fps = frameCountInOneSecond;
		frameCountInOneSecond = 0;
		lastFpsTime = allTime;
	}
	//std::cout << "allTime : " << allTime << std::endl;
}