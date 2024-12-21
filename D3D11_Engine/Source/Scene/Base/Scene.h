#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <Utility/AssimpUtility.h>

class GameObject;
class Scene
{
	friend class SceneManager;
	using InstanceID = unsigned int;
public:
	Scene();
	virtual void Start() {}

	virtual ~Scene();
protected:
	virtual void ImGUIRender() {}

private:
	void ImGUIBegineDraw();
	void ImGUIEndDraw();

public:
	bool UseImGUI = false;

protected:
	/*������ ���� ���ҽ��� ����ϴ�. ��ϵ� ������Ʈ�� �޸𸮴� �� ����� �����˴ϴ�.*/
	void SetResouceObj(const wchar_t* key, GameObject* obj);
	/*�� ���ҽ��� �����մϴ�.*/
	void RemoveResouceObj(const wchar_t* key);
	/*�� ���ҽ��� ���� �����մϴ�.*/
	void ClearResouceObj();
public:
	const wchar_t* GetSceneName() { return sceneName.c_str(); }

private:
	std::wstring sceneName;

private:
	std::unordered_map<std::wstring, std::list<std::shared_ptr<GameObject>>> sceneResourceList;
	
private:
	std::vector<std::shared_ptr<GameObject>> objectList;
	std::vector<std::weak_ptr<GameObject>>   dontdestroyonloadList;
private:
	//Update
	void FixedUpdate();
	void Update();
	void LateUpdate();

	//Render
	void Render();
};