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

	//*********窗口*************
	void CreateWindow(const char* windowName, int width, int height);
	void DestoryWindow(SDL_Window* window);
	void ToggleFullscreen();
	//*************************

	//*********绘制*************
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

	//*********事件*************
	void handleEvents();
	void handleKeyDownEvents(SDL_Keysym * keysym);
	//**************************

	//*********辅助*************
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

