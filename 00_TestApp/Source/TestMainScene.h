#pragma once
#include <Scene/Base/Scene.h>
#include <Core/DXTKInputSystem.h>
#include <Material/BlingPhongMaterial.h>
#include <Material/PBRMaterial.h>
#include <Light/PBRDirectionalLight.h>
#include <map>
#include <Manager/GameObjectFactory.h>

class Camera;
class TestMainScene : public Scene, public InputProcesser
{
public:
	TestMainScene();
	virtual ~TestMainScene() override;
	virtual void ImGUIRender() override;
	virtual void Start() override;

private:
	GameObject* mainCam = nullptr;
	Camera* camera = nullptr;
	float* pCamSpeed = nullptr;

	std::shared_ptr<cb_BlingPhongMaterial> material;
	std::map<std::string, GameObject*> charObjectList;

public:
	void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker,
		const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) override;
};