#pragma once
#include <Scene\Base\Scene.h>
#include <set>

class SimpleMaterial;
struct cb_BlingPhongMaterial;
class ResourceManagerScene : public Scene
{
public:
	ResourceManagerScene();
	virtual ~ResourceManagerScene() override;

protected:
	virtual void ImGUIRender() override;

private:
	float* pCamSpeed = nullptr;

private:
	GameObject* test = nullptr;
	std::vector<GameObject*> testList;
	std::shared_ptr<cb_BlingPhongMaterial> material;

private:
	void AddTestObject();
	void SubTestObject();
	void ClearTestObject();

	void ClearAllResource();
};