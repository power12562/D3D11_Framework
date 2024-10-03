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

void SceneManager::UpdateTransformScene()
{
	currScene->UpdateTransform();
}

void SceneManager::RenderScene()
{
	currScene->Render();
}

void SceneManager::NextSccene()
{
	if (nextScene)
	{
		currScene = std::move(nextScene);
	}
}

void SceneManager::AddObjects()
{
	while (!objectAddQueue.empty())
	{
		GameObject* obj = objectAddQueue.front();
		currScene->objectList.emplace_back(obj);
		objectAddQueue.pop();
	}
}


