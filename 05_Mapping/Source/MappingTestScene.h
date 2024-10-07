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
	float LightDiffuse[4]{ 1, 1, 1, 1 };
	float LightAmbient[4]{ 0, 0, 0, 0 };
	float LightSpecular[4]{ 1, 1, 1, 1 };

	float MaterialAmbient[4]{ 1, 1, 1, 1 };
	float MaterialDiffuse[4]{ 1, 1, 1, 1 };
	float MaterialSpecular[4]{ 1, 1, 1, 1 };

private:
	float bgColor[4]{0,0,0,1};
};