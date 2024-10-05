#include "SceneManager.h"
#include <GameObject\Base\CameraObject.h>

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
			AddObjectNextScene(obj);
			nextAddQueue.pop();
		}
		currScene.reset();
		currScene = std::move(nextScene);
		if (Camera::GetMainCamera() == nullptr)
		{
			auto mainCamera = new CameraObject(L"MainCamera");
			mainCamera->SetMainCamera();
			AddObjectCurrScene(mainCamera);
			currAddQueue.pop();
		}
	}
}

void SceneManager::AddObjectCurrScene(GameObject* obj)
{
	currScene->objectFindMap[obj->Name][obj->GetInstanceID()] = (int)currScene->objectList.size();
	currScene->objectList.emplace_back(obj);
}

void SceneManager::AddObjectNextScene(GameObject* obj)
{
	nextScene->objectFindMap[obj->Name][obj->GetInstanceID()] = (int)nextScene->objectList.size();
	nextScene->objectList.emplace_back(obj);
}

void SceneManager::ChangeObjectName(unsigned int instanceID, const std::wstring& _pervName, const std::wstring& _newName)
{
	if (nextScene)
	{
		auto pervNameIter = nextScene->objectFindMap.find(_pervName);
		if (pervNameIter == nextScene->objectFindMap.end())
			return;
		
		nextScene->objectFindMap[_newName][instanceID] = pervNameIter->second[instanceID];
		nextScene->objectFindMap[_pervName].erase(instanceID);
	}
	else if(currScene)
	{
		auto pervNameIter = currScene->objectFindMap.find(_pervName);
		if(pervNameIter == currScene->objectFindMap.end())
			return;

		currScene->objectFindMap[_newName][instanceID] = pervNameIter->second[instanceID];
		currScene->objectFindMap[_pervName].erase(instanceID);
	}
}

void SceneManager::AddObjects()
{
	while (!currAddQueue.empty())
	{
		GameObject* obj = currAddQueue.front();
		AddObjectCurrScene(obj);
		currAddQueue.pop();
	}
}
						

