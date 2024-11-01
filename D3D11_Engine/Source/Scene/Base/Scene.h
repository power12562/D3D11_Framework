#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <GameObject\Base\GameObject.h>

class Scene
{
	friend class SceneManager;
	using InstanceID = unsigned int;
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
	/*씬에서 사용될 리소스로 만듭니다. 등록된 오브젝트의 메모리는 씬 종료시 삭제됩니다.*/
	void SetResouceObj(GameObject* obj);

private:
	std::vector<std::shared_ptr<GameObject>> sceneResourceList;

protected:
	std::vector<std::shared_ptr<GameObject>> objectList;
	std::unordered_map<std::wstring, std::unordered_set<InstanceID>> objectFindMap;
private:
	//Update
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void UpdateTransform();

	//Render
	void Render();

};