#include "SDLInterface.h"

SDLInterface::SDLInterface(const char* windowName, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LogError("Could not initialize SDL");
	}

	CreateWindow(windowName, width, height);
	CreateRenderer();
}


SDLInterface::~SDLInterface()
{
}

void SDLInterface::CreateWindow(const char* windowName, int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	mainWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	if (mainWindow == NULL)
	{
		LogError("Could not create window");
		Quit(1);
	}


}
void SDLInterface::DestoryWindow(SDL_Window* window)
{
	SDL_DestroyWindow(window);
}

void SDLInterface::LogError(const char* msg)
{
	std::cout << ("%s : %s", msg, SDL_GetError()) << std::endl;
}

void SDLInterface::Quit(int code)
{
	SDL_Quit();
	exit(code);
}

void SDLInterface::handleEvents()
{
	// Our SDL event placeholder.  
	SDL_Event event;
	//Grab all the events off the queue.  
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_KEYDOWN:
			handleKeyDownEvents(&event.key.keysym);
			break;
		case SDL_QUIT:
			// Handle quit requests (like Ctrl-c).  
			Quit(0);
			break;
		default:
			break;
		}
	}
}

void SDLInterface::Render()
{
	SDL_RenderPresent(renderer);
}

void SDLInterface::handleKeyDownEvents(SDL_Keysym* keysym)
{
	switch (keysym->sym)
	{
	case SDLK_ESCAPE:
		Quit(0);
		break;
	case SDLK_F1:
		ToggleFullscreen();
		break;
	default:
		break;
	}
}

void SDLInterface::ToggleFullscreen()
{
	if (windowed)
	{
		int result = SDL_SetWindowFullscreen(mainWindow, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
		if (result < 0) return;
		windowed = false;
	}
	else
	{
		SDL_SetWindowSize(mainWindow, screenWidth, screenHeight);
		windowed = true;
	}

}

void SDLInterface::DrawPixel(Color* color, int posX, int posY)
{
	SetDrawColor(color);
	SDL_RenderDrawPoint(renderer, posX, posY);
}

void SDLInterface::CreateRenderer()
{
	renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
}

void SDLInterface::SetDrawColor(Color* color)
{
	SDL_SetRenderDrawColor(renderer, color->GetUintR(), color->GetUintG(), color->GetUintB(), color->GetUintA());
}

void SDLInterface::RenderClear(Color* color)
{
	SetDrawColor(color);
	SDL_RenderClear(renderer);
}

//Bresenham’s Line Drawing Algorithm
void SDLInterface::DrawLine(Color* color, int x0, int y0, int x1, int y1)
{
	//1 attempt
	/*for (float t = 0.0; t < 1.0; t += 0.01)
	{
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;
		DrawPixel(color, x, y);
	}*/

	//2 attempt
	/*for (int x = x0; x <= x1; x++)
	{
		float t = Mathf::Abs((x - x0) / (float)(x1 - x0));
		int y = y0 + (y1 - y0) * t;
		DrawPixel(color, x, y);
	}*/

	//3 attempt
	bool steep = false;
	if (Mathf::Abs(x1 - x0) < Mathf::Abs(y1 - y0))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	//4 attempt
	//通过误差提高运行效率
	int dyError = Mathf::Abs(dy * 2);
	int error = 0;

	float y = y0;
	for (int x = x0; x <= x1; x++)
	{
		/*float t = Mathf::Abs((x - x0) / (float)(x1 - x0));
		int y = y0 + (y1 - y0) * t;*/

		if (steep)
			DrawPixel(color, y, x);
		else
			DrawPixel(color, x, y);

		error += dyError;
		if (error >  dx)
		{
			y += (y1 > y0) ? 1 : -1;
			error -= 2 * dx;
		}
	}
}

