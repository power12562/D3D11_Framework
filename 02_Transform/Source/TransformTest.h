#pragma once
#include <Framework/WinGameApp.h>

class TransformTest : public WinGameApp
{
public:
	TransformTest();
	~TransformTest();

protected:
	virtual void Start();
	virtual void Update();
	virtual void Render();


};