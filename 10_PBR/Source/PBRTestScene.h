#pragma once
#include <Scene\Base\Scene.h>
#include <set>

struct cb_PBRMaterial;
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
	std::shared_ptr<cb_PBRMaterial> material;
};