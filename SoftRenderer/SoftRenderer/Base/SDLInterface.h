#pragma once

#include<iostream>
#include<SDL.h>

class SDLInterface
{
public:
	SDLInterface();
	~SDLInterface();

	//*********����*************
	void CreateWindow(const char* windowName, int width, int height);
	void DestoryWindow(const char* windowName);
	//*************************

	//*********����*************
	void Render();
	//void DrawPixel(Color* color);
	//**************************

	//*********�¼�*************
	void handleEvents();
	void handleKeyDownEvents(SDL_Keysym * keysym);
	//**************************

	//*********����*************
	void LogError(const char* msg);
	void Quit(int code);
	//**************************

	SDL_Window* mainWindow;
	SDL_Renderer* render;
};

