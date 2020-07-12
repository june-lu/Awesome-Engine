#include "SDLInterface.h"


SDLInterface::SDLInterface(const char* windowName, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LogError("Could not initialize SDL");
	}

	CreateWindow(windowName, width, height);
	CreateRenderer();
	CreateMainTexture();
	CreateSurface();
	keyboardEventHandleCB = std::bind(&SDLInterface::handleKeyDownEvents, this, placeholders::_1);
}


SDLInterface::~SDLInterface()
{
}

void SDLInterface::CreateWindow(const char* windowName, int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	mainWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (mainWindow == NULL)
	{
		LogError("Could not create window");
		Quit(1);
	}
}

void SDLInterface::ChangeWindowName(const char* windowName)
{
	SDL_SetWindowTitle(mainWindow, windowName);
}

void SDLInterface::DestoryWindow(SDL_Window* window)
{
	SDL_DestroyWindow(window);
}

void SDLInterface::CreateMainTexture()
{
	mainRt = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
}
void SDLInterface::CreateSurface()
{
	surface = SDL_GetWindowSurface(mainWindow);
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
			keyboardEventHandleCB(&event.key.keysym);
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
	std::cout << "SDLInterface::handleKeyDownEvents is come" << std::endl;
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

void SDLInterface::CreateRenderer()
{
	renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

void SDLInterface::SwapBuffer(RenderContext* renderContext)
{
	void* mPixels;
	int mPitch;

	

	SDL_LockTexture(mainRt, NULL, &mPixels, &mPitch);

	memcpy(mPixels, &(renderContext->frameBuffer[0]), surface->pitch * surface->h);
	stbi_write_png("stbpng1.png", surface->w, surface->h, 4, &(renderContext->frameBuffer[0]), 4 * surface->w);
	SDL_UnlockTexture(mainRt);	

	SDL_RenderCopy(renderer, mainRt, NULL, NULL);
	
	SDL_RenderPresent(renderer);

}