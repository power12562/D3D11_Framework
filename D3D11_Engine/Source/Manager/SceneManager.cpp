#include "SceneManager.h"
#include <GameObject\Base\CameraObject.h>
#include <Core/DXTKInputSystem.h>
#include <D3DCore/D3D11_GameApp.h>
#include <Component/Render/MeshRender.h>

SceneManager& sceneManager = SceneManager::GetInstance();

void SceneManager::AddGameObject(std::shared_ptr<GameObject>& object)
{
	if (sceneManager.nextScene)
		sceneManager.nextAddQueue.push(object);
	else
		sceneManager.currAddQueue.push(object);
}

SceneManager::SceneManager()
{
	
}

SceneManager::~SceneManager()
{

}

void SceneManager::SetResouceObj(const wchar_t* key, GameObject* obj)
{
	if (nextScene)
	{
		nextScene->SetResouceObj(key, obj);
	}
	else if (currScene)
	{
		currScene->SetResouceObj(key, obj);
	}
	else
	{
		resourceObjectList.push_back(std::make_pair(key, obj));
	}
}

void SceneManager::RemoveResouceObj(const wchar_t* key)
{
	if (nextScene)
	{
		nextScene->RemoveResouceObj(key);
	}
	else if (currScene)
	{
		currScene->RemoveResouceObj(key);
	}
	else
	{
		__debugbreak(); //씬이 존재하지 않음.
	}
}

void SceneManager::ClearResouceObj()
{
	if (nextScene)
	{
		nextScene->ClearResouceObj();
	}
	else if (currScene)
	{
		currScene->ClearResouceObj();
	}
	else
	{
		__debugbreak(); //씬이 존재하지 않음.
	}
}

void SceneManager::DestroyObject(GameObject* obj)
{
	eraseSet.insert(obj);
	if (obj->transform.GetChildCount() > 0)
	{
		for (Transform* childObj : obj->transform.childList)
		{
			DestroyObject(childObj->_gameObject);
		}
	}
}

void SceneManager::DestroyObject(GameObject& obj)
{
	DestroyObject(&obj);
}

void SceneManager::DontDestroyOnLoad(GameObject* obj)
{
	if (nextScene)
	{
		nextScene->dontdestroyonloadList.emplace_back(obj->GetWeakPtr());
	}
	else
	{
		currScene->dontdestroyonloadList.emplace_back(obj->GetWeakPtr());
	}

	if (obj->transform.GetChildCount() > 0)
	{
		for (Transform* childObj : obj->transform.childList)
		{
			DontDestroyOnLoad(childObj->_gameObject);
		}
	}
}

void SceneManager::DontDestroyOnLoad(GameObject& obj)
{
	DontDestroyOnLoad(&obj);
}

size_t SceneManager::GetObjectsCount()
{
	if (currScene)
	{
		size_t count = 0;
		for (auto& item : currScene->objectList)
		{
			if (item)
				++count;
		}
		return count;
	}
	else return 0;
}

ObjectList SceneManager::GetObjectList()
{
	if (currScene)
	{
		std::vector<GameObject*> objList(GetObjectsCount());
		int i = 0;
		for (auto& item : currScene->objectList)
		{
			if (item)
			{
				objList[i] = item.get();
				++i;
			}
		}
		return objList;
	}
	else return std::vector<GameObject*>();
}

void SceneManager::BindImGUIPopupFunc(const std::function<void()>& func)
{
	if (nextScene)
		nextScene->ImGUIPopupFunc = func;
	else if (currScene)
		currScene->ImGUIPopupFunc = func;
}

void SceneManager::ResetImGUIPopupFunc()
{
	if (nextScene)
		nextScene->ImGUIPopupFunc = nullptr;
	else if (currScene)
		currScene->ImGUIPopupFunc = nullptr;
}

GameObject* SceneManager::FindObject(const wchar_t* name)
{
	GameObject* obj = nullptr;
	auto findIter = objectFindMap.find(name);
	if (findIter != objectFindMap.end())
	{
		int firstID = *(findIter->second.begin());
		obj = currScene->objectList[firstID].get();
	}
	return obj;
}

GameObject* SceneManager::GetObjectToID(unsigned int instanceID)
{
	GameObject* obj = nullptr;
	if (currScene && instanceID < currScene->objectList.size())
	{
		obj = currScene->objectList[instanceID].get();
	}
	return obj;
}

std::vector<GameObject*> SceneManager::FindObjects(const wchar_t* name)
{
	std::vector<GameObject*> objects;
	auto findIter = objectFindMap.find(name);
	if(findIter != objectFindMap.end())
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

void SceneManager::RenderScene()
{
	if(Camera::GetMainCamera())
		currScene->Render();
}

void SceneManager::AddObjects()
{
	while (!currAddQueue.empty())
	{
		auto& obj = currAddQueue.front();
		AddObjectCurrScene(obj);
		obj->transform.UpdateTransform();
		obj->transform.ResetFlagUpdateWM();	
		currAddQueue.pop();
	}
}

void SceneManager::EraseObjects()
{
	if (!eraseSet.empty())
	{
		for (auto& obj : eraseSet)
		{
			EraseObject(EraseObjectFindMap(obj));
		}
		eraseSet.clear();

		auto& objList = currScene->objectList;
		while (!objList.empty() && objList.back() == nullptr)
		{
			objList.pop_back();
		}
	}
}

void SceneManager::ChangeScene()
{
	if (nextScene)
	{
		while (!nextAddQueue.empty())
		{
			auto& obj = nextAddQueue.front();
			AddObjectNextScene(obj);
			nextAddQueue.pop();
		}
		if (currScene)
		{
			currScene->ClearResouceObj();
			std::erase_if(currScene->dontdestroyonloadList, [](std::weak_ptr<GameObject> ptr) {return ptr.expired(); });
			if (!currScene->dontdestroyonloadList.empty())
			{
				nextScene->dontdestroyonloadList = std::move(currScene->dontdestroyonloadList);
				for (auto& weakptr : nextScene->dontdestroyonloadList)
				{
					unsigned int id = weakptr.lock()->GetInstanceID();
					if (nextScene->objectList.size() <= id)
						nextScene->objectList.resize(id + 1);

					nextScene->objectList[id] = currScene->objectList[id];
				}
			}
			currScene.reset();
			MeshRender::ReloadShaderAll(); //유효한 메시 객체들 셰이더 다시 생성
		}
		currScene = std::move(nextScene);
		currScene->Start();
		
		if (Camera::GetMainCamera() == nullptr)
		{
			auto mainCamera = std::make_shared<CameraObject>();
			mainCamera->SetMainCamera();
			mainCamera->Name = L"MainCamera";
			mainCamera->instanceID = instanceIDManager.getUniqueID();
			auto obj = std::static_pointer_cast<GameObject>(mainCamera);
			AddObjectCurrScene(obj);
		}
		AddObjects();
	}
	else if (EndGame)
	{
		WinGameApp::GameEnd();
		currScene.reset();
		nextScene.reset();
	}
}

void SceneManager::AddObjectCurrScene(std::shared_ptr<GameObject>& obj)
{
	unsigned int id = obj->GetInstanceID();
	objectFindMap[obj->Name].insert(id);
	if (id >= currScene->objectList.size())
	{
		currScene->objectList.resize(id + 1);
	}
	currScene->objectList[id] = obj;
}

void SceneManager::AddObjectNextScene(std::shared_ptr<GameObject>& obj)
{
	unsigned int id = obj->GetInstanceID();
	objectFindMap[obj->Name].insert(id);
	if (id >= nextScene->objectList.size())
	{
		nextScene->objectList.resize(id + 1);
	}
	nextScene->objectList[id] = obj;
}

void SceneManager::ChangeObjectName(unsigned int instanceID, const std::wstring& _pervName, const std::wstring& _newName)
{
	auto pervNameIter = objectFindMap.find(_pervName);
	if (pervNameIter == objectFindMap.end())
		return;

	objectFindMap[_newName].insert(instanceID);
	objectFindMap[_pervName].erase(instanceID);
}

void SceneManager::EraseObject(unsigned int id)
{
	currScene->objectList[id].reset(); //오브젝트 삭제
}

unsigned int SceneManager::EraseObjectFindMap(GameObject* obj)
{
	auto findIter = objectFindMap.find(obj->Name);
	if (findIter == objectFindMap.end())
		return -1;

	unsigned int index = obj->GetInstanceID();
	findIter->second.erase(obj->GetInstanceID()); //맵에서 삭제

	return index;
}


