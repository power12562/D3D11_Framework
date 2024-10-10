#include "SceneManager.h"
#include <GameObject\Base\CameraObject.h>
#include <Framework\DXTKInputSystem.h>

SceneManager& sceneManager = SceneManager::GetInstance();

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::DestroyObject(GameObject* obj)
{
	eraseSet.insert(obj);
}

void SceneManager::DestroyObject(GameObject& obj)
{
	eraseSet.insert(&obj);
}

GameObject* SceneManager::FindObject(const wchar_t* name)
{
	GameObject* obj = nullptr;
	auto findIter = currScene->objectFindMap.find(name);
	if (findIter != currScene->objectFindMap.end())
	{
		int firstID = *(findIter->second.begin());
		obj = currScene->objectList[firstID].get();
	}
	return obj;
}

std::vector<GameObject*> SceneManager::FindObjects(const wchar_t* name)
{
	std::vector<GameObject*> objects;
	auto findIter = currScene->objectFindMap.find(name);
	if(findIter != currScene->objectFindMap.end())
	{
		for (auto index : findIter->second)
		{
			objects.push_back(currScene->objectList[index].get());
		}
	}
	return objects;
}

void SceneManager::FixedUpdateScene()
{
	currScene->FixedUpdate();
}

void SceneManager::UpdateScene()
{
	DXTKinputSystem.Update();
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

void SceneManager::AddObjects()
{
	while (!currAddQueue.empty())
	{
		GameObject* obj = currAddQueue.front();
		AddObjectCurrScene(obj);
		currAddQueue.pop();
	}
}

void SceneManager::EraseObjects()
{
	if (!eraseSet.empty())
	{
		for (auto& obj : eraseSet)
		{
			EraseObject(obj);
		}
		eraseSet.clear();
	}
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
			auto mainCamera = NewGameObject<CameraObject>(L"MainCamera");
			mainCamera->SetMainCamera();
			AddObjectCurrScene(mainCamera);
			currAddQueue.pop();
		}
	}
}

void SceneManager::AddObjectCurrScene(GameObject* obj)
{
	int id = obj->GetInstanceID();
	currScene->objectFindMap[obj->Name].insert(id);
	if (id < currScene->objectList.size())
		currScene->objectList[id].reset(obj);
	else
		currScene->objectList.emplace_back(obj);
}

void SceneManager::AddObjectNextScene(GameObject* obj)
{
	int id = obj->GetInstanceID();
	nextScene->objectFindMap[obj->Name].insert(id);
	if (id < nextScene->objectList.size())
		nextScene->objectList[id].reset(obj);
	else
		nextScene->objectList.emplace_back(obj);
}

void SceneManager::ChangeObjectName(unsigned int instanceID, const std::wstring& _pervName, const std::wstring& _newName)
{
	if (nextScene)
	{
		auto pervNameIter = nextScene->objectFindMap.find(_pervName);
		if (pervNameIter == nextScene->objectFindMap.end())
			return;
		
		nextScene->objectFindMap[_newName].insert(instanceID);
		nextScene->objectFindMap[_pervName].erase(instanceID);
	}
	else if(currScene)
	{
		auto pervNameIter = currScene->objectFindMap.find(_pervName);
		if(pervNameIter == currScene->objectFindMap.end())
			return;

		currScene->objectFindMap[_newName].insert(instanceID);
		currScene->objectFindMap[_pervName].erase(instanceID);
	}
}

void SceneManager::EraseObject(GameObject* obj)
{
	if (nextScene)
	{
		auto findIter = nextScene->objectFindMap.find(obj->Name);
		if (findIter == nextScene->objectFindMap.end())
			return;

		int index = obj->GetInstanceID();
		findIter->second.erase(obj->GetInstanceID()); //맵에서 삭제
		nextScene->objectList[index].reset(); //오브젝트 삭제
	}
	else if (currScene)
	{
		auto findIter = currScene->objectFindMap.find(obj->Name);
		if (findIter == currScene->objectFindMap.end())
			return;

		int index = obj->GetInstanceID();
		findIter->second.erase(obj->GetInstanceID()); //맵에서 삭제
		currScene->objectList[index].reset(); //오브젝트 삭제
	}
}

