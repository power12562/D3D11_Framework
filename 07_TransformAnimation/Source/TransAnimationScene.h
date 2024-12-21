#pragma once
#include <Scene\Base\Scene.h>
#include <Material/BlingPhongMaterial.h>

class TransAnimationScene : public Scene
{
public:
	TransAnimationScene() = default;
	virtual ~TransAnimationScene() override;
	virtual void Start() override;
protected:
	virtual void ImGUIRender() override;

private:
	float* pCamSpeed = nullptr;
	std::shared_ptr<cb_BlingPhongMaterial> material_01;

private:
	GameObject* box = nullptr;
};