#pragma once
#include <Scene/Base/Scene.h>
#include <map>

class Camera;
class CameraMoveHelper;
class SkyBoxObject;
class SphereObject;
class cb_PBRMaterial;
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

	GameObject* pistol = nullptr;
	std::map<std::string, cb_PBRMaterial*> pistolMaterials;

	SphereObject* Sphere = nullptr;
	cb_PBRMaterial* sphereMaterial = nullptr;	
};