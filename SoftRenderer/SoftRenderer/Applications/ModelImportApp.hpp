#pragma once
#include "AppBase.hpp"
#include "Base/Model.h"

class ModelImportApp : public AppBase
{
public:
	ModelImportApp(std::string appName, int appWidth, int appHeight) :AppBase(appName, appWidth, appHeight) {};
	void Run()
	{
		Shader shader;
		Model model("ModelData/nanosuit.obj", renderManager);
		renderManager->RenderClear();
		model.Draw(&shader);

		renderManager->SwapBuffer();

		renderManager->handleEvents();
	}
	~ModelImportApp()
	{
	}

};