#pragma once
#include <Scene\Base\Scene.h>

class TransAnimationScene : public Scene
{
public:
	TransAnimationScene();
	virtual ~TransAnimationScene() override;

protected:
	virtual void ImGUIRender() override;

private:
	float* pCamSpeed = nullptr;

private:
	GameObject* test1 = nullptr;
	GameObject* test2 = nullptr;
	GameObject* test3 = nullptr;
};