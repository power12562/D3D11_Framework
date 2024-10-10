#pragma once
#include <Scene\Base\Scene.h>
#include <Framework\TSingleton.h>
#include <GameObject/Base/GameObject.h>
#include <unordered_map>
#include <string>
#include <functional>
#include <queue>
#include <set>

extern class SceneManager& sceneManager;
class SceneManager : public TSingleton<SceneManager>
{
	friend TSingleton;
	friend class D3D11_GameApp;
	friend const std::wstring& GameObject::SetName(const wchar_t* _name);
	friend LRESULT CALLBACK ImGUIWndProcDefault(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	
	template<typename ObjectType>
	static ObjectType* NewGameObject(const wchar_t* name);

private:
	SceneManager();
	~SceneManager();

	std::unique_ptr<Scene> currScene;
	std::unique_ptr<Scene> nextScene;

	std::queue<GameObject*> currAddQueue; //게임 오브젝트 추가 대기열
	std::queue<GameObject*>	nextAddQueue; //게임 오브젝트 추가 대기열

	std::set<GameObject*> eraseSet;		  //게임 오브젝트 삭제 대기열
public:
	template <typename T>
	void LoadScene();

	void DestroyObject(GameObject* obj);
	void DestroyObject(GameObject& obj);

	GameObject* FindObject(const wchar_t* name);
	std::vector<GameObject*> FindObjects(const wchar_t* name);

private:
	//Update
	void FixedUpdateScene();
	void UpdateScene();
	void LateUpdateScene();
	void UpdateTransformScene();

	//Render
	void RenderScene();
	void AddObjects();
	void EraseObjects();
	void NextSccene();
	
private:
	void AddObjectCurrScene(GameObject* obj);
	void AddObjectNextScene(GameObject* obj);
	void ChangeObjectName(unsigned int instanceID, const std::wstring& _pervName, const std::wstring& _newName);
	void EraseObject(GameObject* obj);
};

template<typename ObjectType>
ObjectType* NewGameObject(const wchar_t* name)
{
	return SceneManager::NewGameObject<ObjectType>(name);
}

#include <Framework\InstanceIDManager.h>
template<typename ObjectType>
inline ObjectType* SceneManager::NewGameObject(const wchar_t* name)
{
	static_assert(std::is_base_of_v<GameObject, ObjectType>, "is not gameObject");

	ObjectType* newObject = new ObjectType;

	if (sceneManager.nextScene)
		sceneManager.nextAddQueue.push(newObject);
	else
		sceneManager.currAddQueue.push(newObject);

	newObject->Name = name;
	newObject->instanceID = instanceIDManager.getUniqueID();

	return newObject;
}

template<typename T>
inline void SceneManager::LoadScene()
{
	static_assert(std::is_base_of_v<Scene, T>, "T is not Scene");

	nextScene.reset(new T);
}
