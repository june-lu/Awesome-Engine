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

	//*********窗口*************
	void CreateWindow(const char* windowName, int width, int height);
	void DestoryWindow(SDL_Window* window);
	void ToggleFullscreen();
	//*************************

	//*********纹理**************
	void CreateMainTexture();
	void CreateSurface();
	//**************************

	//*********绘制*************
	void CreateRenderer();
	void Render();
	void SwapBuffer(RenderContext* context);
	void SetDrawColor(Color* color);
	void RenderClear(Color* color);
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
	SDL_Renderer* renderer;
	SDL_Texture* mainRt;
	SDL_Surface* surface;
	int screenHeight;
	int screenWidth;
	bool windowed = false;
};

