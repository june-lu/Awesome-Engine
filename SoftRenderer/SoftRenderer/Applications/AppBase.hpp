#pragma once
#include<string>
#include "RenderSystem/RenderManager.h"

class AppBase
{
public:
	AppBase(std::string appName, int appWidth, int appHeight)
	{
		renderManager = new RenderManager(appName.c_str(), appWidth, appHeight);
	}
	~AppBase() {}

	virtual void Init() = 0;
	virtual void Run() = 0;
	virtual void Release() = 0;


protected:
	RenderManager* renderManager;
};
