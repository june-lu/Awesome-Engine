#include "RenderSystem/RenderManager.h"


int main(int argc, char *argv[])
{
	
	RenderManager* renderManager = new RenderManager("SoftRender", 800, 600);

	//sdlInterface->RenderClear(&Color::white);

	Vector2f t0[3] = { Vector2f(10, 70),   Vector2f(50, 160),  Vector2f(70, 80) };
	Vector2f t1[3] = { Vector2f(180, 50),  Vector2f(150, 1),   Vector2f(70, 180) };
	Vector2f t2[3] = { Vector2f(180, 150), Vector2f(120, 160), Vector2f(130, 180) };

	//Vector2f pos4 = Vector2f(40, 80);

	//sdlInterface->DrawLine(&Color::white, pos1, pos2);
	//sdlInterface->DrawLine(&Color::red, pos3, pos4);
	//sdlInterface->DrawLine(&Color::green, pos2, pos1);
		
	renderManager->DrawTriangleByBarycentricCoordinates(&Color::red, t0);
	renderManager->DrawTriangleByBarycentricCoordinates(&Color::white, t1);
	renderManager->DrawTriangleByBarycentricCoordinates(&Color::green, t2);

	renderManager->SwapBuffer();

	while (true)
	{
		renderManager->handleEvents();
	}

	getchar();
	return 0;

}