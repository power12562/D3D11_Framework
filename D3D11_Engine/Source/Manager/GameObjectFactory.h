#pragma once
#include <Core/TSingleton.h>
#include <Manager/SceneManager.h>
#include <map>
#include <string>
#include <functional>
#include <Core/StaticBlockMemoryPool.h>

/*������Ʈ Ŭ���� ����� ���� ��ũ�� ����*/
#define SERIALIZED_OBJECT(TypeName)	\
friend class GameObjectFactory;	\
inline static bool TypeName##FactoryInit = GameObjectFactory::AddNewObjectFuntion<TypeName>(); \

class GameObject;
class GameObjectFactory;
extern GameObjectFactory& gameObjectFactory;

class GameObjectFactory : public TSingleton<GameObjectFactory>
{
	friend class TSingleton;
    inline static std::map<std::string, std::function<GameObject*(const wchar_t* name)>> newGameObjectFuncMap;
	inline static size_t MaxGameObjectClassSize = 0;
	inline static std::string MaxSizeGameObjectClassName;
public:
	//���� ������Ʈ Ŀ���� ������
	static void GameObjectDeleter(GameObject* pObj);

	template<typename T>
	static bool AddNewObjectFuntion()
	{
		static_assert(std::is_base_of_v<GameObject, T>, "T is not gameObject");
		std::string key = typeid(T).name();

		auto func = [](const wchar_t* name) { return NewGameObject<T>(name); };
		newGameObjectFuncMap[key] = func;
		if (MaxGameObjectClassSize < sizeof(T))
		{
			MaxGameObjectClassSize = sizeof(T);
			MaxSizeGameObjectClassName = key;
		}
		return true;
	}

private:
	GameObjectFactory() = default;
	~GameObjectFactory() override;

public:
	void Initialize();
	void* GameObjectAlloc(size_t id);
	std::function<GameObject*(const wchar_t* name)>& NewGameObjectToKey(const char* key);

private:
	StaticBlockMemoryPool gameObjectMemoryPool;
};

