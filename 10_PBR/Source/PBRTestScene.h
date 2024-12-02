#pragma once
#include <Scene\Base\Scene.h>
#include <set>

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
};