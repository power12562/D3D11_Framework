#pragma once
#include <Scene\Base\Scene.h>
#include <set>
#include <Material/PBRMaterial.h>
#include <Light\PBRDirectionalLight.h>
#include <GameObject/Mesh/PBRMeshObject.h>

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
	std::unordered_map<std::string, PBRMeshObject*> cerberusObjList;
	std::unordered_map<std::string, PBRMeshObject*> charObjList;

	cb_PBRMaterial* sphereMaterial;
};