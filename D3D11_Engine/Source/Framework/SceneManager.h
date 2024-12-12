#pragma once
#include <Scene\Base\Scene.h>
#include <Framework\TSingleton.h>
#include <GameObject/Base/GameObject.h>
#include <unordered_map>
#include <string>
#include <functional>
#include <queue>
#include <set>
#include <Utility/AssimpUtility.h>
#include <Utility/utfConvert.h>

extern class SceneManager& sceneManager;
using ObjectList = const std::vector<GameObject*>;
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

	std::queue<std::shared_ptr<GameObject>> currAddQueue; //���� ������Ʈ �߰� ��⿭
	std::queue<std::shared_ptr<GameObject>>	nextAddQueue; //���� ������Ʈ �߰� ��⿭

	std::set<GameObject*> eraseSet;		  //���� ������Ʈ ���� ��⿭

	std::list<std::pair<std::wstring, GameObject*>> resourceObjectList; //���ҽ��� ����� ������Ʈ ��⿭
public:
	template <typename T>
	void LoadScene();

	/*������ ���� ���ҽ��� ����ϴ�. ��ϵ� ������Ʈ�� �޸𸮴� �� ����� �����˴ϴ�.*/
	void SetResouceObj(const wchar_t* key, GameObject* obj);

	/*���ҽ����� �����մϴ�.*/
	void RemoveResouceObj(const wchar_t* key);

	/*���� �ִ� ���ҽ��� ���� �����մϴ�.*/
	void ClearResouceObj();

	void DestroyObject(GameObject* obj);
	void DestroyObject(GameObject& obj);

	GameObject* FindObject(const wchar_t* name);
	GameObject* GetObjectToID(unsigned int instanceID);
	std::vector<GameObject*> FindObjects(const wchar_t* name);

	size_t GetObjectsCount();
	ObjectList GetObjectList();

	bool EndGame = false;
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
	void AddObjectCurrScene(std::shared_ptr<GameObject> obj);
	void AddObjectNextScene(std::shared_ptr<GameObject> obj);
	void ChangeObjectName(unsigned int instanceID, const std::wstring& _pervName, const std::wstring& _newName);
	void EraseObject(GameObject* obj);
};

template<typename ObjectType = GameObject>
ObjectType* NewGameObject(const wchar_t* name)
{
	return SceneManager::NewGameObject<ObjectType>(name);
}

#include <Framework\InstanceIDManager.h>
template<typename ObjectType>
inline ObjectType* SceneManager::NewGameObject(const wchar_t* name)
{
	static_assert(std::is_base_of_v<GameObject, ObjectType>, "is not gameObject");

	std::shared_ptr<ObjectType> newObject = std::make_shared<ObjectType>();

	if (sceneManager.nextScene)
		sceneManager.nextAddQueue.push(std::static_pointer_cast<GameObject>(newObject));
	else
		sceneManager.currAddQueue.push(std::static_pointer_cast<GameObject>(newObject));

	newObject->Name = name;
	newObject->instanceID = instanceIDManager.getUniqueID();
	newObject->myptr = newObject;

	return newObject.get();
}

template<typename T>
inline void SceneManager::LoadScene()
{
	static_assert(std::is_base_of_v<Scene, T>, "T is not Scene");

	nextScene.reset(new T);
	nextScene->sceneName = utfConvert::utf8_to_wstring(typeid(T).name());
	for (auto& pair : resourceObjectList)
	{
		nextScene->SetResouceObj(pair.first.c_str(), pair.second);
	}
	resourceObjectList.clear();
}
