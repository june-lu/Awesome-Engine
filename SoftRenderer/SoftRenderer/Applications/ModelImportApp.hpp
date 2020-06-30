#pragma once
#include "AppBase.hpp"
#include "Base/Model.h"
#include "Base/Camera.h"
#include "Base/Time.h"

class ModelImportApp : public AppBase
{
public:
	ModelImportApp(std::string appName, int appWidth, int appHeight)
	{
		this->appName = appName;
		renderManager = new RenderManager(appName.c_str(), appWidth, appHeight);
		shader = new DefaultShader();
	}

	void Init()
	{
		model = Model("ModelData/nanosuit.obj", renderManager);
		camera = Camera({ 0, 0, 70 }, { 0, 1, 0 }, { 0, 0, 1 });
		renderManager->SetCamera(camera);
		renderManager->sdlInterface->keyboardEventHandleCB = std::bind(&ModelImportApp::handleKeyDownEvents, this, placeholders::_1);
		time.Start();
		model.Draw(shader);
	}

	~ModelImportApp()
	{
		Release();
	}

	void Run()
	{
		time.Update();
		renderManager->sdlInterface->ChangeWindowName((appName + " (FPS: " + to_string(Time::fps) + ")").c_str());
		renderManager->handleEvents();
		renderManager->SetCamera(camera);
		renderManager->RenderClear();
		renderManager->rasterizer->DrawTriangleByBarycentricCoordinates();
		renderManager->SwapBuffer();
	}

	void Release()
	{
		delete renderManager;
		delete shader;
	}

	void handleKeyDownEvents(SDL_Keysym* keysym)
	{
		switch (keysym->sym)
		{
		case SDLK_ESCAPE:
			renderManager->sdlInterface->Quit(0);
			break;
		case SDLK_F1:
			renderManager->sdlInterface->ToggleFullscreen();
			break;
		case SDLK_w:
			camera.MoveFront(1);
			break;
		case SDLK_s:
			camera.MoveFront(-1);
			break;
		case SDLK_a:
			camera.MoveLeft(1);
			break;
		case SDLK_d:
			camera.MoveLeft(-1);
			break;
		default:
			break;
		}
	}


private:
	Shader* shader;
	Model model;
	Camera camera;
	RenderManager* renderManager;
	Time time;
	string appName;
};

