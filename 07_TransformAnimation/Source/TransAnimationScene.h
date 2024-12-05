#pragma once
#include <Scene\Base\Scene.h>
#include <Material/BlingPhongMaterial.h>

class TransAnimationScene : public Scene
{
public:
	TransAnimationScene();
	virtual ~TransAnimationScene() override;

protected:
	virtual void ImGUIRender() override;

private:
	float* pCamSpeed = nullptr;
	std::shared_ptr<cb_BlingPhongMaterial> material_01;

private:
	GameObject* box = nullptr;
};