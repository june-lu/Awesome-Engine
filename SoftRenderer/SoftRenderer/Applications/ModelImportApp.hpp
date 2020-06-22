#pragma once
#include "AppBase.hpp"
#include "Base/Model.h"
#include "Base/Camera.h"

class ModelImportApp : public AppBase
{
public:
	ModelImportApp(std::string appName, int appWidth, int appHeight)
		:AppBase(appName, appWidth, appHeight)
	{
	}

	void Init()
	{
		model = Model("ModelData/nanosuit.obj", renderManager);
		camera = new Camera({ 0, 0, 60 }, { 0, 1, 0 }, { 0, 0, 1 });
		renderManager->SetCamera(*camera);
	}

	~ModelImportApp()
	{
		Release();
	}

	void Run()
	{

		renderManager->RenderClear();
		model.Draw(&shader);

		renderManager->SwapBuffer();

		renderManager->handleEvents();
	}

	void Release()
	{
		delete camera;
	}
private:
	Shader shader;
	Model model;
	Camera* camera;
};