#pragma once
#include <Scene\Base\Scene.h>

class ImportFBXScene : public Scene
{
public:
	ImportFBXScene() = default;
	virtual ~ImportFBXScene() override;
	virtual void Start() override;
protected:
	virtual void ImGUIRender() override;

private:
	float* pCamSpeed = nullptr;
};