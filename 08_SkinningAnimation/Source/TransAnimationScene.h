#pragma once
#include <Scene\Base\Scene.h>

struct cb_BlingPhongMaterial;
class TransAnimationScene : public Scene
{
public:
	TransAnimationScene();
	virtual ~TransAnimationScene() override;

protected:
	virtual void ImGUIRender() override;

private:
	float* pCamSpeed = nullptr;
	std::shared_ptr<cb_BlingPhongMaterial> material;

private:
	GameObject* test1 = nullptr;
	GameObject* test2 = nullptr;
	GameObject* test3 = nullptr;
};