#pragma once
#include <SDL.h>
#include <iostream>
class Time
{
public:
	Time();
	~Time();
	void Start();
	void Update();

	static int fps;
	static float detalTime;
	static float allTime;

	int frameCountInOneSecond;
	float startTime;
	float lastUpdateTime;
	float lastFpsTime;

};

