#pragma once
#include <Scene/Base/Scene.h>
#include <map>
#include <string>

class ShadowTestScene : public Scene
{
public:
	ShadowTestScene() = default;
	virtual ~ShadowTestScene() override = default;
	virtual void Start() override;

	virtual void ImGUIRender();

private:
	std::map<std::string, struct cb_PBRMaterial*> pistolMaterials;
	std::map<std::string, class PBRMeshObject*> charMaterials;
	cb_PBRMaterial* sphereMaterial = nullptr;
private:
	class Camera* pCamera = nullptr;
	class CameraMoveHelper* pCameraMoveHelper = nullptr;
	class SkyBoxObject* skyBox = nullptr;
	class GameObject* cube = nullptr;
	void AddObjects(size_t positionZcount);
};