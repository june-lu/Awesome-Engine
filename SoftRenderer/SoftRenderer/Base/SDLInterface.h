#pragma once

#include<iostream>
#include<SDL.h>
#include "Color.h"
#include "Vector2.h"
#include "Vector3.h"

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

	//*********����*************
	void CreateRenderer();
	void Render();
	void DrawPixel(Color* color, int posX, int poxY);
	void DrawLine(Color* color, int x0, int y0,int x1, int y1);
	void DrawLine(Color* color, Vector2f start, Vector2f end);
	void DrawTriangleByLineSweeping(Color* color, Vector2f* pts);
	void DrawTriangleByBarycentricCoordinates(Color* color, Vector2f* pts);
	void SwapBuffer();
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

private:
	SDL_Window* mainWindow;
	SDL_Renderer* renderer;
	int screenHeight;
	int screenWidth;
	bool windowed = false;
};

