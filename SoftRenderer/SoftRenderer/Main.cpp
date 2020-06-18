#include "RenderSystem/RenderManager.h"
#include "Base/Model.h"
int main(int argc, char *argv[])
{
	RenderManager* renderManager = new RenderManager("SoftRender", 1000, 800);

	//Vector2f center(400, 300);
	//int length = 50;
	//float angle = 180;
	//Vector3f t0[3] = { Vector3f(center.x - length, center.y - length, -length),   Vector3f(center.x + length, center.y - length, -length),  Vector3f(center.x - length, center.y + length, -length) };
	//Vector3f t1[3] = { Vector3f(center.x + length, center.y + length, -length),   Vector3f(center.x + length, center.y - length, -length),  Vector3f(center.x - length, center.y + length, -length) };

	//Vector3f t2[3] = { Vector3f(center.x - length, center.y - length, length),   Vector3f(center.x + length, center.y - length, length),  Vector3f(center.x - length, center.y + length, length) };
	//Vector3f t3[3] = { Vector3f(center.x + length, center.y + length, length),   Vector3f(center.x + length, center.y - length, length),  Vector3f(center.x - length, center.y + length, length) };

	//for (int i = 0; i < 3; i++)
	//{
	//	t0[i] = t0[i].RotationY(angle);
	//	t1[i] = t1[i].RotationY(angle);
	//	t2[i] = t2[i].RotationY(angle);
	//	t3[i] = t3[i].RotationY(angle);
	//}

	////前面
	//renderManager->DrawTriangleByBarycentricCoordinates(&Color::red, t0, ShaderMode::ShadedWireframe);
	//renderManager->DrawTriangleByBarycentricCoordinates(&Color::red, t1, ShaderMode::ShadedWireframe);

	////后面
	//renderManager->DrawTriangleByBarycentricCoordinates(&Color::blue, t2, ShaderMode::ShadedWireframe);
	//renderManager->DrawTriangleByBarycentricCoordinates(&Color::blue, t3, ShaderMode::ShadedWireframe);

	Shader shader;
	Model model("ModelData/Crate/Crate1.obj", renderManager);
	renderManager->RenderClear();
	model.Draw(&shader);

	renderManager->SwapBuffer();

	while (true)
	{
		renderManager->handleEvents();
	}

	getchar();
	return 0;

}