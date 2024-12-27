#pragma once
#include <Core/TSingleton.h>
#include <Utility/AssimpUtility.h>
#include <unordered_map>
#include <string>
#include <functional>
#include <queue>
#include <set>
#include <Utility/utfConvert.h>
#include <Scene\Base\Scene.h>

extern class SceneManager& sceneManager;
using ObjectList = std::vector<GameObject*>;
class SceneManager : public TSingleton<SceneManager>
{
	friend TSingleton;
	friend class WinGameApp;
	friend class D3D11_GameApp;
	friend class GameObject;
	friend LRESULT CALLBACK ImGUIWndProcDefault(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	SceneManager();
	~SceneManager();

	std::unique_ptr<Scene> currScene;
	std::unique_ptr<Scene> nextScene;

	std::queue<std::shared_ptr<GameObject>> currAddQueue; //���� ������Ʈ �߰� ��⿭
	std::queue<std::shared_ptr<GameObject>>	nextAddQueue; //���� ������Ʈ �߰� ��⿭

	std::set<GameObject*> eraseSet;		  //���� ������Ʈ ���� ��⿭

	std::list<std::pair<std::wstring, GameObject*>> resourceObjectList; //���ҽ��� ����� ������Ʈ ��⿭

	std::map<std::wstring, std::unordered_set<Scene::InstanceID>> objectFindMap;
public:
	template <typename T>
	void LoadScene();

	/*���� ���� ������Ʈ �߰�*/
	void AddGameObject(std::shared_ptr<GameObject>& object);

	/*������ ���� ���ҽ��� ����ϴ�. ��ϵ� ������Ʈ�� �޸𸮴� �� ����� �����˴ϴ�.*/
	void SetResouceObj(const wchar_t* key, GameObject* obj);

	/*���ҽ����� �����մϴ�.*/
	void RemoveResouceObj(const wchar_t* key);

	/*���� �ִ� ���ҽ��� ���� �����մϴ�.*/
	void ClearResouceObj();

	void DestroyObject(GameObject* obj);
	void DestroyObject(GameObject& obj);

	void DontDestroyOnLoad(GameObject* obj);
	void DontDestroyOnLoad(GameObject& obj);

	GameObject* FindObject(const wchar_t* name);
	GameObject* GetObjectToID(unsigned int instanceID);
	std::vector<GameObject*> FindObjects(const wchar_t* name);

	size_t GetObjectsCount();
	ObjectList GetObjectList();
	
	/*���� �� Imgui ��� ���� ��ȯ*/
	bool IsImGuiActive();
	void PushImGuiPopupFunc(const std::function<void()>& func);
	void PopImGuiPopupFunc();

	bool EndGame = false;
private:
	//Update
	void FixedUpdateScene();
	void UpdateScene();
	void LateUpdateScene();

	//Render
	void RenderScene();
	void AddObjects();
	void EraseObjects();
	void ChangeScene();
	
private:
	void AddObjectCurrScene(std::shared_ptr<GameObject>& obj);
	void AddObjectNextScene(std::shared_ptr<GameObject>& obj);
	void ChangeObjectName(unsigned int instanceID, const std::wstring& _pervName, const std::wstring& _newName);
	void EraseObject(unsigned int id);
	unsigned int EraseObjectFindMap(GameObject* obj);
};

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
