#pragma once
#include <Scene/Base/Scene.h>
class MappingTestScene : public Scene
{
public:
	MappingTestScene();
	virtual	~MappingTestScene() override;

	void ImGUIRender() override;

private:
	float cubeScale{1};

private:
	float LightDir[4]{ 0, 0, 1, 0 };

};