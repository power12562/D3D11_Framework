#pragma once
#include <string>
#include <vector>
#include <memory>
#include <GameObject\Base\GameObject.h>

class Scene
{
	friend class SceneManager;
public:
	Scene();
	virtual ~Scene();

protected:
	virtual void ImGUIRender() {}

private:
	void ImGUIBegineDraw();
	void ImGUIEndDraw();

public:
	bool UseImGUI = false;

protected:
	std::vector<std::shared_ptr<GameObject>> objectList;

private:
	//Update
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void UpdateTransform();

	//Render
	void Render();

};