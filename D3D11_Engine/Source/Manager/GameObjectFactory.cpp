#include "GameObjectFactory.h"
#include <GameObject/Base/GameObject.h>
#include <framework.h>
#include <Utility/SerializedUtility.h>

GameObjectFactory& gameObjectFactory = GameObjectFactory::GetInstance();

GameObjectFactory::~GameObjectFactory()
{
	gameObjectMemoryPool.Uninitialize();
}

void GameObjectFactory::GameObjectDeleter(GameObject* pObj)
{
	unsigned int id = pObj->GetInstanceID();
	pObj->~GameObject();
	gameObjectFactory.gameObjectMemoryPool.Free(id);
}

void GameObjectFactory::InitializeMemoryPool()
{
	gameObjectMemoryPool.Initialize(MaxGameObjectClassSize, MaxGameObjectClassSize * 1000);
}

void GameObjectFactory::UninitializeMemoryPool()
{
	gameObjectMemoryPool.Uninitialize();
}

void* GameObjectFactory::GameObjectAlloc(size_t id)
{
	return gameObjectMemoryPool.Allocate(id);
}

std::function<GameObject*(const wchar_t* name)>& GameObjectFactory::NewGameObjectToKey(const char* key)
{
	auto findIter = newGameObjectFuncMap->find(key);
	if (findIter != newGameObjectFuncMap->end())
	{
		return findIter->second;
	}
	__debugbreak(); //존재하지 않는 키.
	throw std::runtime_error("Key not found in the map");
}

void GameObjectFactory::SerializedObject(GameObject* object, const wchar_t* WritePath)
{
	if (object == nullptr)
	{
		__debugbreak(); //존재하지 않는 오브젝트
		return;
	}

	if (newGameObjectFuncMap->find(typeid(*object).name()) == newGameObjectFuncMap->end())
	{
		MessageBox(NULL, L"Is not Serialized Object", object->Name.c_str(), MB_OK);
		return;
	}
	std::filesystem::path path(WritePath);
	if (path.extension() != L".GameObject")
	{
		if (!Utility::IsPathDirectory(path))
		{
			path += L"/";
		}
		path += object->GetName();
		path += L".GameObject";
	}

	if (!std::filesystem::exists(path))
	{
		if (!std::filesystem::exists(path.parent_path()))
		{
			std::filesystem::create_directories(path.parent_path());
		}
	}
	else
	{
		int result = MessageBox(
			NULL,                      
			L"파일이 존재합니다. 덮어쓰시겠습니까?", 
			path.c_str(),
			MB_OKCANCEL | MB_ICONQUESTION 
		);
		if (result == IDCANCEL) {
			return;
		}
	}	

	std::ofstream ofs(path.c_str(), std::ios::binary | std::ios::trunc);
	Serialized(object, ofs, 0);
	ofs.close();
}

void GameObjectFactory::DeserializedObject(const wchar_t* ReadPath)
{
	std::filesystem::path path(ReadPath);
	if (!std::filesystem::exists(path))
	{
		MessageBox(NULL, L"파일이 존재하지 않습니다.", ReadPath, MB_OK);
		return;
	}
	if (path.extension() != L".GameObject")
	{
		MessageBox(NULL, L"GameObject 파일이 아닙니다.", ReadPath, MB_OK);
		return;
	}

	std::ifstream ifs(path.c_str(), std::ios::binary);
	Deserialized(ifs);
	ifs.close();
	return;
}

void GameObjectFactory::Serialized(GameObject* object, std::ofstream& ofs, size_t level)
{
	using namespace Binary;
	const char* typeName = typeid(*object).name();
	Write::string(ofs, typeName);
	const wchar_t* objName = object->Name.c_str();
	Write::wstring(ofs, objName);
	Write::data(ofs, object->Active);
	Write::BoundingBox(ofs, object->Bounds);
	object->Serialized(ofs);
	//Components
	for (auto& component : object->componentList)
	{
		component->Serialized(ofs);
	}
	//Transform
	Write::data<size_t>(ofs, level);
	bool isParent = level > 0;
	if (!isParent)
	{
		Write::Vector3(ofs, object->transform.position);
		Write::Quaternion(ofs, object->transform.rotation);
		Write::Vector3(ofs, object->transform.scale);
	}
	else
	{
		Write::Vector3(ofs, object->transform.localPosition);
		Write::Quaternion(ofs, object->transform.localRotation);
		Write::Vector3(ofs, object->transform.localScale);
	}	
	//childs
	for (size_t i = 0; i < object->transform.GetChildCount(); i++)
	{
		if(GameObject* chidObject = sceneManager.GetObjectToID(object->transform.GetChild(i)->gameObject.GetInstanceID())) //존재하는지 검증
			Serialized(chidObject, ofs, level + 1);
	}
}

void GameObjectFactory::Deserialized(std::ifstream& ifs)
{
	static std::map<size_t, Transform*> objectTreeMap;
	using namespace Binary;
	while (true)
	{
		std::string typeName = Read::string(ifs);
		if (ifs.eof())
		{
			objectTreeMap.clear();
			return;
		}
		std::wstring objName = Read::wstring(ifs);
		GameObject* object = NewGameObjectToKey(typeName.c_str())(objName.c_str());
		object->Active = Read::data<bool>(ifs);
		object->Bounds = Read::BoundingBox(ifs);
		object->Deserialized(ifs);
		//Components
		for (auto& component : object->componentList)
		{
			component->Deserialized(ifs);
		}
		//Transform
		size_t myLevel = Read::data<size_t>(ifs);
		objectTreeMap[myLevel] = &object->transform;
		bool isParent = myLevel > 0;
		if (isParent)
		{
			auto find = objectTreeMap.find(myLevel - 1); //마지막으로 생성된 내 상위 객체를 찾는다
			if (find != objectTreeMap.end())
				object->transform.SetParent(find->second); //찾으면 부모로 설정
		}
		if (!isParent)
		{
			object->transform.position = Read::Vector3(ifs);
			object->transform.rotation = Read::Quaternion(ifs);
			object->transform.scale	   = Read::Vector3(ifs);
		}
		else
		{
			object->transform.localPosition = Read::Vector3(ifs);
			object->transform.localRotation = Read::Quaternion(ifs);
			object->transform.localScale	= Read::Vector3(ifs);
		}
	}
}
 