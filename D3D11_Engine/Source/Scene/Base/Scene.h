#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <GameObject\Base\GameObject.h>

class Scene
{
	friend class SceneManager;
	using InstanceID = unsigned int;
	using Index = int;
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
	std::unordered_map<std::wstring, std::unordered_map<InstanceID, Index>> objectFindMap;
private:
	//Update
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void UpdateTransform();

	//Render
	void Render();

};