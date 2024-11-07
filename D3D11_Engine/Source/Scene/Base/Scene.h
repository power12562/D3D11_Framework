#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <GameObject\Base\GameObject.h>
#include <Utility/AssimpUtility.h>

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
	/*������ ���� ���ҽ��� ����ϴ�. ��ϵ� ������Ʈ�� �޸𸮴� �� ����� �����˴ϴ�.*/
	void SetResouceObj(const wchar_t* key, GameObject* obj);
	/*�� ���ҽ��� �����մϴ�.*/
	void RemoveResouceObj(const wchar_t* key);
	/*�� ���ҽ��� ���� �����մϴ�.*/
	void ClearResouceObj();
private:
	std::unordered_map<std::wstring, std::list<std::shared_ptr<GameObject>>> sceneResourceList;

private:
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