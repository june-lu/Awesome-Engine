#pragma once
class AppBase
{
public:
	AppBase() {}
	~AppBase() {}

	virtual void Init() = 0;
	virtual void Run() = 0;
	virtual void Release() = 0;


protected:

};
