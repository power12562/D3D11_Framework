#pragma once
#include <Scene/Base/Scene.h>
#include <Material/BlingPhongMaterial.h>

class TestMainScene : public Scene
{
public:
	TestMainScene();
	virtual ~TestMainScene() override;
	virtual void ImGUIRender() override;

private:
	GameObject* mainCam;
	std::shared_ptr<cb_BlingPhongMaterial> material;
};