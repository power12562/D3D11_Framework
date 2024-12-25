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

void GameObjectFactory::Initialize()
{
	gameObjectMemoryPool.Initialize(MaxGameObjectClassSize, MaxGameObjectClassSize * 1000);
}

void* GameObjectFactory::GameObjectAlloc(size_t id)
{
	return gameObjectMemoryPool.Allocate(id);
}

std::function<GameObject*(const wchar_t* name)>& GameObjectFactory::NewGameObjectToKey(const char* key)
{
	auto findIter = newGameObjectFuncMap.find(key);
	if (findIter != newGameObjectFuncMap.end())
	{
		return findIter->second;
	}
	__debugbreak(); //�������� �ʴ� Ű.
	throw std::runtime_error("Key not found in the map");
}

void GameObjectFactory::SerializedObject(GameObject* object, const wchar_t* WritePath)
{
	if (newGameObjectFuncMap.find(typeid(*object).name()) == newGameObjectFuncMap.end())
	{
		MessageBox(NULL, L"Is not Serialized Object", object->Name.c_str(), MB_OK);
		return;
	}
	std::filesystem::path path(WritePath);
	if (!Utility::IsPathDirectory(path))
	{
		path += L"/";
	}
	path += object->GetName();
	path += L".GameObject";
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
			L"������ �����մϴ�. ����ðڽ��ϱ�?", 
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
		MessageBox(NULL, L"������ �������� �ʽ��ϴ�.", ReadPath, MB_OK);
		return;
	}
	if (path.extension() != L".GameObject")
	{
		MessageBox(NULL, L"GameObject ������ �ƴմϴ�.", ReadPath, MB_OK);
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
	Write::Vector3(ofs, object->transform.position);
	Write::Quaternion(ofs, object->transform.rotation);
	Write::Vector3(ofs, object->transform.scale);
	Write::data<size_t>(ofs, level); 
	//childs
	for (size_t i = 0; i < object->transform.GetChildCount(); i++)
	{
		Serialized(&object->transform.GetChild(i)->gameObject, ofs, level + 1);
	}
}

void GameObjectFactory::Deserialized(std::ifstream& ifs)
{
	static std::map<size_t, Transform*> objectTreeMap;
	using namespace Binary;
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
	object->transform.position = Read::Vector3(ifs);
	object->transform.rotation = Read::Quaternion(ifs);
	object->transform.scale = Read::Vector3(ifs);
	size_t myLevel = Read::data<size_t>(ifs);
	objectTreeMap[myLevel] = &object->transform;
	if (myLevel > 0)
	{
		auto find = objectTreeMap.find(myLevel - 1); //���������� ������ �� ���� ��ü�� ã�´�
		if (find != objectTreeMap.end())
			object->transform.SetParent(find->second); //ã���� �θ�� ����
	}
	//Childs
	Deserialized(ifs);
}
 