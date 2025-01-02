#include "GameObjectFactory.h"
#include <GameObject/Base/GameObject.h>
#include <framework.h>
#include <Utility/SerializedUtility.h>
#include <Component/Render/SimpleBoneMeshRender.h>

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
	__debugbreak(); //�������� �ʴ� Ű.
	throw std::runtime_error("Key not found in the map");
}

void GameObjectFactory::SerializedObject(GameObject* object, const wchar_t* WritePath)
{
	if (object == nullptr)
	{
		__debugbreak(); //�������� �ʴ� ������Ʈ
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

GameObject* GameObjectFactory::DeserializedObject(const wchar_t* ReadPath)
{
	std::filesystem::path path(ReadPath);
	if (!std::filesystem::exists(path))
	{
		MessageBox(NULL, L"������ �������� �ʽ��ϴ�.", ReadPath, MB_OK);
		return nullptr;
	}
	if (path.extension() != L".GameObject")
	{
		MessageBox(NULL, L"GameObject ������ �ƴմϴ�.", ReadPath, MB_OK);
		return nullptr;
	}

	std::ifstream ifs(path.c_str(), std::ios::binary);
	GameObject* obj = Deserialized(ifs);
	ifs.close();
	return obj;
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

	//TransformAnimation
	TransformAnimation* animation = object->IsComponent<TransformAnimation>();
	bool isTransformAnimation = animation != nullptr;
	Write::data(ofs, isTransformAnimation);
	if (animation)
		animation->SerializedAnimation(ofs);

	//childs
	for (size_t i = 0; i < object->transform.GetChildCount(); i++)
	{
		if(GameObject* chidObject = &object->transform.GetChild(i)->gameObject)
			Serialized(chidObject, ofs, level + 1);
	}
}

GameObject* GameObjectFactory::Deserialized(std::ifstream& ifs)
{
	static std::map<size_t, Transform*> objectTreeMap;
	std::vector<TransformAnimation*> transformAnimationVec;
	GameObject* root = nullptr;
	using namespace Binary;
	while (true)
	{
		std::string typeName = Read::string(ifs);
		if (ifs.eof())
		{
			objectTreeMap.clear();
			for (auto& animation : transformAnimationVec)
			{
				animation->AddChildrenToTargets();
			}
			SimpleBoneMeshRender::EndDeserialized();
			return root;
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
			auto find = objectTreeMap.find(myLevel - 1); //���������� ������ �� ���� ��ü�� ã�´�
			if (find != objectTreeMap.end())
				object->transform.SetParent(find->second); //ã���� �θ�� ����
		}
		if (!isParent)
		{
			object->transform.position = Read::Vector3(ifs);
			object->transform.rotation = Read::Quaternion(ifs);
			object->transform.scale	   = Read::Vector3(ifs);
			root = object;
		}
		else
		{
			object->transform.localPosition = Read::Vector3(ifs);
			object->transform.localRotation = Read::Quaternion(ifs);
			object->transform.localScale	= Read::Vector3(ifs);
		}

		//TransformAnimation
		bool isTransformAnimation = Read::data<bool>(ifs);
		if (isTransformAnimation)
		{
			TransformAnimation* animation = &object->AddComponent<TransformAnimation>();
			animation->DeserializedAnimation(ifs);
			transformAnimationVec.push_back(animation);
		}
	}
}
 