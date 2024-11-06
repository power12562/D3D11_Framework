#pragma once
#include <Scene/Base/Scene.h>

class TestMainScene : public Scene
{
public:
	TestMainScene();
	virtual ~TestMainScene() override;
	virtual void ImGUIRender() override;
};