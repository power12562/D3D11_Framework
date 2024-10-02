#include "SceneManager.h"

SceneManager& sceneManager = SceneManager::GetInstance();

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::FixedUpdateScene()
{
	currScene->FixedUpdate();
}

void SceneManager::UpdateScene()
{
	currScene->Update();
}

void SceneManager::LateUpdateScene()
{
	currScene->LateUpdate();
}

void SceneManager::NextSccene()
{
	if (nextScene)
	{
		currScene = std::move(nextScene);
	}
}


