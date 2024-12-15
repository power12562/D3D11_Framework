#include "GameObjectFactory.h"

GameObjectFactory& gameObjectFactory = GameObjectFactory::GetInstance();

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
