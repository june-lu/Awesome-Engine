#pragma once

#include<iostream>
#include<SDL.h>

class SDLInterface
{
public:
	SDLInterface();
	~SDLInterface();

	//*********窗口*************
	void CreateWindow(const char* windowName, int width, int height);
	void DestoryWindow(const char* windowName);
	//*************************

	//*********绘制*************
	void Render();
	//void DrawPixel(Color* color);
	//**************************

	//*********事件*************
	void handleEvents();
	void handleKeyDownEvents(SDL_Keysym * keysym);
	//**************************

	//*********辅助*************
	void LogError(const char* msg);
	void Quit(int code);
	//**************************

	SDL_Window* mainWindow;
	SDL_Renderer* render;
};

