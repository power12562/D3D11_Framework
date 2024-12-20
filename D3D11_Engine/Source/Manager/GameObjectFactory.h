#pragma once
#include <Core/TSingleton.h>
#include <Manager/SceneManager.h>
#include <map>
#include <string>
#include <functional>

/*오브젝트 클래스 선언시 다음 매크로 포함*/
#define SERIALIZED_OBJECT(TypeName)	\
friend class GameObjectFactory;	\
inline static bool TypeName##FactoryInit = gameObjectFactory.AddNewObjectFuntion<TypeName>();

class GameObject;
class GameObjectFactory;
extern GameObjectFactory& gameObjectFactory;

class GameObjectFactory : public TSingleton<GameObjectFactory>
{
	friend class TSingleton;
    inline static std::map<std::string, std::function<GameObject*(const wchar_t* name)>> newGameObjectFuncMap;
private:
	GameObjectFactory() = default;
	~GameObjectFactory() = default;

public:
	template<typename T>
	bool AddNewObjectFuntion()
	{
		static_assert(std::is_base_of_v<GameObject, T>, "T is not gameObject");
		std::string key = typeid(T).name();

		auto func = [](const wchar_t* name) { return NewGameObject<T>(name); };
		newGameObjectFuncMap[key] = func;
		return true;
	}

	std::function<GameObject*(const wchar_t* name)>& NewGameObjectToKey(const char* key);
};