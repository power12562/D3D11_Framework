#pragma once
#include <Scene/Base/Scene.h>
#include <map>

class Camera;
class CameraMoveHelper;
class SkyBoxObject;
class SphereObject;
class PBRMeshObject;
struct cb_PBRMaterial;
class IBLTestScene : public Scene
{
public:
	IBLTestScene();
	virtual ~IBLTestScene() override;
	virtual void Start() override;

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

	GameObject* cha = nullptr;
	std::map<std::string, PBRMeshObject*> charMaterials;
};