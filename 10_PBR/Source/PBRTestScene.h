#pragma once
#include <Scene\Base\Scene.h>
#include <map>
#include <Material/PBRMaterial.h>
#include <Light\PBRDirectionalLight.h>
#include <GameObject/Mesh/PBRMeshObject.h>

class SimpleMaterial;
class PBRTestScene : public Scene
{
public:
	PBRTestScene() = default;
	virtual ~PBRTestScene() override;

	virtual void Start() override;
protected:
	virtual void ImGUIRender() override;

private:
	float* pCamSpeed = nullptr;
	std::map<std::string, PBRMeshObject*> cerberusObjList;
	std::map<std::string, PBRMeshObject*> charObjList;

	cb_PBRMaterial* sphereMaterial;
};