#pragma once
#include <Scene\Base\Scene.h>

struct cb_BlingPhongMaterial;
class SkinningAnimationScene : public Scene
{
public:
	SkinningAnimationScene();
	virtual ~SkinningAnimationScene() override;

protected:
	virtual void ImGUIRender() override;

private:
	float* pCamSpeed = nullptr;

private:
	GameObject* test1 = nullptr;
	GameObject* test2 = nullptr;
	GameObject* test3 = nullptr;

public:
	std::list<cb_BlingPhongMaterial> MaterialList; //std::vector 사용시 emplace_back() 할때마다 주소가 이동된다. 따라서 list 사용
};