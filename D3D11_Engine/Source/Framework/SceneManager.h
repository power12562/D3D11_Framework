#pragma once
#include <Scene\Base\Scene.h>
#include <Framework\TSingleton.h>
#include <unordered_map>
#include <string>
#include <functional>

extern class SceneManager& sceneManager;
class SceneManager : public TSingleton<SceneManager>
{
	friend TSingleton;
	friend class D3D11_GameApp;
private:
	SceneManager();
	~SceneManager();

	std::unique_ptr<Scene> currScene;
	std::unique_ptr<Scene> nextScene;
public:
	template <typename T>
	void LoadScene();
private:
	void FixedUpdateScene();
	void UpdateScene();
	void LateUpdateScene();

	void NextSccene();
	
};

template<typename T>
inline void SceneManager::LoadScene()
{
	static_assert(std::is_base_of_v<Scene, T>, "T is not Scene");
	nextScene.reset(new T);
}
