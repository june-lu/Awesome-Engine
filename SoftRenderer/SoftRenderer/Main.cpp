#include <SDL.h>
#include "Base/SDLInterface.h"
#include "Base/Vector2.h"

int main(int argc, char *argv[])
{
	SDLInterface* sdlInterface = new SDLInterface("SoftRender", 800, 600);

	sdlInterface->RenderClear(&Color::black);

	Vector2f t0[3] = { Vector2f(10, 70),   Vector2f(50, 160),  Vector2f(70, 80) };
	Vector2f t1[3] = { Vector2f(180, 50),  Vector2f(150, 1),   Vector2f(70, 180) };
	Vector2f t2[3] = { Vector2f(180, 150), Vector2f(120, 160), Vector2f(130, 180) };

	//Vector2f pos4 = Vector2f(40, 80);

	//sdlInterface->DrawLine(&Color::white, pos1, pos2);
	//sdlInterface->DrawLine(&Color::red, pos3, pos4);
	//sdlInterface->DrawLine(&Color::green, pos2, pos1);
		
	sdlInterface->DrawTriangleByBarycentricCoordinates(&Color::red, t0);
	sdlInterface->DrawTriangleByBarycentricCoordinates(&Color::white, t1);
	sdlInterface->DrawTriangleByBarycentricCoordinates(&Color::green, t2);

	sdlInterface->Render();

	while (true)
	{
		sdlInterface->handleEvents();
	}

	getchar();
	return 0;

}