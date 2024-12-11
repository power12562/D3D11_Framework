#pragma once
#include <Scene/Base/Scene.h>

class Camera;
class CameraMoveHelper;
class SkyBoxObject;
class IBLTestScene : public Scene
{
public:
	IBLTestScene();
	virtual ~IBLTestScene() override;

protected:
	virtual void ImGUIRender() override;

private:
	Camera* pCamera = nullptr;
	CameraMoveHelper* pCameraMoveHelper = nullptr;
	SkyBoxObject* skyBox = nullptr;
};