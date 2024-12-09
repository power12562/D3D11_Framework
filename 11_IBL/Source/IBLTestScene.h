#pragma once
#include <Scene/Base/Scene.h>

class IBLTestScene : public Scene
{
public:
	IBLTestScene();
	virtual ~IBLTestScene() override;

protected:
	virtual void ImGUIRender() override;
};