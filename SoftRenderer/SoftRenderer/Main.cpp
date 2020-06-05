#include "RenderSystem/RenderManager.h"


int main(int argc, char *argv[])
{
	
	RenderManager* renderManager = new RenderManager("SoftRender", 800, 600);

	//sdlInterface->RenderClear(&Color::white);

	Vector3f t0[3] = { Vector3f(0, 0, 3),   Vector3f(0, 150, 3),  Vector3f(150, 150, 3) };
	Vector3f t1[3] = { Vector3f(150, 0, 4),  Vector3f(0, 150, 10),  Vector3f(150, 150, 2) };
	Vector3f t2[3] = { Vector3f(0, 150, 1), Vector3f(75, 0, 51), Vector3f(150, 150, 1) };

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