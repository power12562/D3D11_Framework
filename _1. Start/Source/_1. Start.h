#pragma once
#include <Framework/WinGameApp.h>

class StartApp : public WinGameApp
{
public:
	StartApp();
	~StartApp();

protected:
	virtual void Update();
	virtual void Render();


};