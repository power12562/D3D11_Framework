#pragma once
#include <Scene\Base\Scene.h>

class ImportFBXScene : public Scene
{
public:
	ImportFBXScene();
	virtual ~ImportFBXScene() override;

protected:
	virtual void ImGUIRender() override;

private:
	float* pCamSpeed = nullptr;
};