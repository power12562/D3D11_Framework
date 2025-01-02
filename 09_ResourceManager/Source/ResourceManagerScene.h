#pragma once
#include <Scene\Base\Scene.h>
#include <Core/DXTKInputSystem.h>
#include <set>

class SimpleMaterial;
struct cb_BlingPhongMaterial;
class ResourceManagerScene : public Scene, public InputProcesser
{
public:
	ResourceManagerScene() = default;
	virtual ~ResourceManagerScene() override;
	virtual void Start() override;
protected:
	virtual void ImGUIRender() override;
	virtual void OnInputProcess(DXTKInputSystem::InputSystem& input);
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