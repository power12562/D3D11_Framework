#pragma once
#include <Scene/Base/Scene.h>
#include <Material/BlingPhongMaterial.h>
#include <map>

class Camera;
class TestMainScene : public Scene
{
public:
	TestMainScene();
	virtual ~TestMainScene() override;
	virtual void ImGUIRender() override;

private:
	GameObject* mainCam;
	Camera* camera = nullptr;
	float* pCamSpeed = nullptr;

	std::shared_ptr<cb_BlingPhongMaterial> material;
	std::list<cb_BlingPhongMaterial> charMaterialList;
	std::map<std::string, GameObject*> charObjectList;
};