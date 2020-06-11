#include "RenderSystem/RenderManager.h"
#include "Base/Model.h"
int main(int argc, char *argv[])
{
	RenderManager* renderManager = new RenderManager("SoftRender", 800, 600);

	//sdlInterface->RenderClear(&Color::white);

	/*Vector3f t0[3] = { Vector3f(-50, 0, 3),   Vector3f(-50, 150, 3),  Vector3f(150, 150, 3) };
	Vector3f t1[3] = { Vector3f(150, 0, 4),  Vector3f(0, 150, 10),  Vector3f(150, 150, 2) };
	Vector3f t2[3] = { Vector3f(0, 150, 1), Vector3f(75, 0, 51), Vector3f(150, 150, 1) };
	
	renderManager->DrawTriangleByBarycentricCoordinates(&Color::red, t0, ShaderMode::Wireframe);
	renderManager->DrawTriangleByBarycentricCoordinates(&Color::white, t1, ShaderMode::Wireframe);
	renderManager->DrawTriangleByBarycentricCoordinates(&Color::green, t2, ShaderMode::Wireframe);*/
	
	Shader shader;
	Model model("ModelData/nanosuit.obj", renderManager);
	model.Draw(&shader);

	renderManager->SwapBuffer();

	while (true)
	{
		renderManager->handleEvents();
	}

	getchar();
	return 0;

}