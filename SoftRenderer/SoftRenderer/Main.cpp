#include <SDL.h>
#include "Base/SDLInterface.h"

int main(int argc, char *argv[])
{
	SDLInterface* sdlInterface = new SDLInterface("SoftRender", 800, 600);

	sdlInterface->RenderClear(&Color::black);

	sdlInterface->DrawLine(&Color::white, 13, 20, 80, 40);
	sdlInterface->DrawLine(&Color::red, 20, 13, 40, 80);
	//sdlInterface->DrawLine(&Color::green, 80, 40, 13, 20);

	sdlInterface->Render();

	while (true)
	{
		sdlInterface->handleEvents();
	}

	getchar();
	return 0;
}