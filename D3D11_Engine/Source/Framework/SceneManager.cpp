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
		while (!nextAddQueue.empty())
		{
			GameObject* obj = nextAddQueue.front();
			nextScene->objectList.emplace_back(obj);
			nextAddQueue.pop();
		}
		currScene = std::move(nextScene);
		nextScene = nullptr;
	}
}

void SceneManager::AddObjects()
{
	while (!currAddQueue.empty())
	{
		GameObject* obj = currAddQueue.front();
		currScene->objectList.emplace_back(obj);
		currAddQueue.pop();
	}
}
						

