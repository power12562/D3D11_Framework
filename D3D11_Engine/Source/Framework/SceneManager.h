#pragma once
#include <Scene\Base\Scene.h>
#include <Framework\TSingleton.h>
#include <GameObject/Base/GameObject.h>
#include <unordered_map>
#include <string>
#include <functional>
#include <queue>

extern class SceneManager& sceneManager;
class SceneManager : public TSingleton<SceneManager>
{
	friend TSingleton;
	friend class D3D11_GameApp;
	friend GameObject::GameObject(const wchar_t* name);
private:
	SceneManager();
	~SceneManager();

	std::unique_ptr<Scene> currScene;
	std::unique_ptr<Scene> nextScene;

	std::queue<GameObject*> objectAddQueue; //게임 오브젝트 추가 대기열
public:
	template <typename T>
	void LoadScene();

private:
	//Update
	void FixedUpdateScene();
	void UpdateScene();
	void LateUpdateScene();
	void UpdateTransformScene();

	//Render
	void RenderScene();
	void NextSccene();
	void AddObjects();
	
};

template<typename T>
inline void SceneManager::LoadScene()
{
	static_assert(std::is_base_of_v<Scene, T>, "T is not Scene");
	nextScene.reset(new T);
}
