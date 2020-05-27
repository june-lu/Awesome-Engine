#include <SDL.h>
#include "SDLInterface.h"

int main(int argc, char *argv[])
{
	SDLInterface* sdlInterface = new SDLInterface();
	sdlInterface->CreateWindow("SoftRender", 800, 600);

	while (true)
	{
		sdlInterface->handleEvents();
	}

	getchar();
	return 0;
}

