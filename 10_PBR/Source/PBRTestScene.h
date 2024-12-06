#pragma once
#include <Scene\Base\Scene.h>
#include <set>
#include <Material/PBRMaterial.h>
#include <Light\PBRDirectionalLight.h>

class SimpleMaterial;
class PBRTestScene : public Scene
{
public:
	PBRTestScene();
	virtual ~PBRTestScene() override;

protected:
	virtual void ImGUIRender() override;

private:
	float* pCamSpeed = nullptr;
	std::shared_ptr<cb_PBRMaterial> shared_material;
	std::unordered_map<std::string, cb_PBRMaterial> charMaterialList;
	std::unordered_map<std::string, GameObject*> charObjList;

	cb_PBRMaterial sphereMaterial;
	cb_PBRDirectionalLight directionalLight;
};