#pragma once
#include <Scene\Base\Scene.h>
#include <Material/BlingPhongMaterial.h>

class SkinningAnimationScene : public Scene
{
public:
	SkinningAnimationScene() = default;
	virtual ~SkinningAnimationScene() override;
	virtual void Start() override;
protected:
	virtual void ImGUIRender() override;

private:
	float* pCamSpeed = nullptr;

private:
	GameObject* test1 = nullptr;
	GameObject* test2 = nullptr;
	GameObject* test3 = nullptr;
};