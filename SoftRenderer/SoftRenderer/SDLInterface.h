#pragma once

#include<iostream>
#include<SDL.h>

class SDLInterface
{
public:
	SDLInterface();
	~SDLInterface();

	void CreateWindow();

	void CreateWindow(const char* windowName, int width, int height);
	void DestoryWindow(const char* windowName);

	void LogError(const char* msg);
	void Quit(int code);
	void handleEvents();

	SDL_Window* mainWindow;
};

