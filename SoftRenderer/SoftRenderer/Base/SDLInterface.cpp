#include "SDLInterface.h"

SDLInterface::SDLInterface()
{
}


SDLInterface::~SDLInterface()
{
}

void SDLInterface::CreateWindow(const char* windowName, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LogError("Could not initialize SDL");
	}

	mainWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	if (mainWindow == NULL)
	{
		LogError("Could not create window");
		Quit(1);
	}


}
void SDLInterface::DestoryWindow(const char* windowName)
{

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
	SDL_RenderPresent(render);
}

void SDLInterface::handleKeyDownEvents(SDL_Keysym * keysym)
{
	switch (keysym->sym)
	{
	case SDLK_ESCAPE:
		Quit(0);
		break;
	default:
		break;
	}
}