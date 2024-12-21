#pragma once
#include <Scene/Base/Scene.h>
class MappingTestScene : public Scene
{
public:
	MappingTestScene() = default;
	virtual	~MappingTestScene() override;
	virtual void Start() override;

	void ImGUIRender() override;

private:
	float cubeScale{1};

private:
	float LightDir[4]{ 0, 0, 1, 0 };

private:
	GameObject* cube1;
};