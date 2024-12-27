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

	std::queue<std::shared_ptr<GameObject>> currAddQueue; //게임 오브젝트 추가 대기열
	std::queue<std::shared_ptr<GameObject>>	nextAddQueue; //게임 오브젝트 추가 대기열

	std::set<GameObject*> eraseSet;		  //게임 오브젝트 삭제 대기열

	std::list<std::pair<std::wstring, GameObject*>> resourceObjectList; //리소스로 등록할 오브젝트 대기열

	std::map<std::wstring, std::unordered_set<Scene::InstanceID>> objectFindMap;
public:
	template <typename T>
	void LoadScene();

	/*현재 씬에 오브젝트 추가*/
	void AddGameObject(std::shared_ptr<GameObject>& object);

	/*씬에서 사용될 리소스로 만듭니다. 등록된 오브젝트의 메모리는 씬 종료시 삭제됩니다.*/
	void SetResouceObj(const wchar_t* key, GameObject* obj);

	/*리소스에서 제거합니다.*/
	void RemoveResouceObj(const wchar_t* key);

	/*씬에 있는 리소스를 전부 정리합니다.*/
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
	
	/*현재 씬 Imgui 사용 여부 반환*/
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
