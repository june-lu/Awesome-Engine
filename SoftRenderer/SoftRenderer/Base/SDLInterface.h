#pragma once

#include<iostream>
#include<SDL.h>
#include "Color.h"
#include "RenderSystem/RenderContext.h"

class SDLInterface
{
public:
	SDLInterface(const char* windowName, int width, int height);
	~SDLInterface();

	//*********����*************
	void CreateWindow(const char* windowName, int width, int height);
	void DestoryWindow(SDL_Window* window);
	void ToggleFullscreen();
	//*************************

	//*********����**************
	void CreateMainTexture();
	void CreateSurface();
	//**************************

	//*********����*************
	void CreateRenderer();
	void Render();
	void SwapBuffer(RenderContext* context);
	void SetDrawColor(Color* color);
	void RenderClear(Color* color);
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
	SDL_Renderer* renderer;
	SDL_Texture* mainRt;
	SDL_Surface* surface;
	int screenHeight;
	int screenWidth;
	bool windowed = false;
};

