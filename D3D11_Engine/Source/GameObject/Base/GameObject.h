#pragma once
#include <Core/Transform.h>
#include <Utility/Console.h>
#include <memory>
#include <string>
#include <Manager/GameObjectFactory.h>

#pragma warning( disable : 4267)

class Component;
class RenderComponent;
class GameObject
{		 
	SERIALIZED_OBJECT(GameObject)
	friend class Scene;
	friend class SceneManager;
	friend class D3DRenderer;
public:
	static void Destroy(GameObject& obj);
	static void Destroy(GameObject* obj);
	static GameObject* Find(const wchar_t* name);
	template<typename ObjectType>
	inline static ObjectType* NewGameObject(const wchar_t* name);

public:
	GameObject(); 
	virtual ~GameObject();

public:
	unsigned int GetInstanceID() const { return instanceID; }
	const std::wstring& GetName() const { return name; }
	const std::wstring& SetName(const wchar_t* _name);
	std::string GetNameToString() const;
public:
	/*오브젝트 이름. (중복 가능)*/
	_declspec (property(get = GetName, put = SetName)) std::wstring& Name;
	Transform transform;
	bool Active = true;

public:
	/*컴포넌트 추가*/
	template <typename T>
	T& AddComponent();

	/*컴포넌트 가져오기*/
	template <typename T>
	T& GetComponent();

	/*컴포넌트 가져오기. (없으면 nullptr 반환)*/
	template <typename T>
	T* IsComponent();

	/*인덱스로 컴포넌트 가져오기. 파라미터로 캐스팅할 컴포넌트 타입을 전달.*/
	template <typename T>
	T* GetComponentAtIndex(int index);

	/*이 오브젝트의 컴포넌트 개수*/
	int GetComponentCount() { return componentList.size(); }

	void DestroyComponent(Component& component);

	void DestroyComponent(Component* component);

	DirectX::BoundingOrientedBox GetOBBToWorld() const;

	//오브젝트 BB 초기 center 값은 Mathf::FLOAT_MAX로 초기화 됩니다.
	DirectX::BoundingBox Bounds;

	/*카메라 컬링 여부*/
	inline bool IsCameraCulling() const { return isCulling; }
private:
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void Render();

private:
	std::vector<std::unique_ptr<Component>> componentList;
	std::vector<RenderComponent*> renderList;

private:
	unsigned int instanceID = -1;
	std::wstring name;
	bool isCulling = false; //이번 프레임 카메라 컬링 여부

private:
	void UpdateChildActive(Transform* rootTransform);

public:
	std::weak_ptr<GameObject> GetWeakPtr() { return myptr; }
private:
	std::weak_ptr<GameObject> myptr;

private:
	bool checkActive = true;
	inline bool CheckActive() const { return checkActive != Active; }
};

template<typename T>
inline T& GameObject::AddComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "is not Component");

	T* nComponent = new T;
	nComponent->SetOwner(this);
	nComponent->Start();
	nComponent->index = componentList.size();
	componentList.emplace_back(nComponent);	
	if constexpr (std::is_base_of_v<RenderComponent, T>)
	{
		renderList.push_back(nComponent);
	}
	return *nComponent;
}

#pragma warning(disable : 4715)
template<typename T>
inline T& GameObject::GetComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "is not Component");

	for (auto& component : componentList)
	{
		if (typeid(*component) == typeid(T))
		{
			return dynamic_cast<T&>(*component);
		}
	}
	__debugbreak(); //예외) 존재하지 않는 컴포넌트
}
#pragma warning(default : 4715)

template<typename T>
inline T* GameObject::IsComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "is not Component");

	for (auto& component : componentList)
	{
		if (typeid(*component) == typeid(T))
		{
			return dynamic_cast<T*>(component.get());
		}
	}
	return nullptr;
}

template<typename T>
inline T* GameObject::GetComponentAtIndex(int index)
{
	static_assert(std::is_base_of_v<Component, T>, "is not Component");

	if (0 <= index && index < componentList.size())
	{
		T* component = dynamic_cast<T*>(componentList[index].get());
		return component;
	}
	Debug_printf("warrnig : GetComponentAtIndex(int index), index is out of range!\n");
	return nullptr;
}

template<typename ObjectType = GameObject>
ObjectType* NewGameObject(const wchar_t* name)
{
	return GameObject::NewGameObject<ObjectType>(name);
}

#include <Manager/InstanceIDManager.h>
template<typename ObjectType>
inline ObjectType* GameObject::NewGameObject(const wchar_t* name)
{
	static_assert(std::is_base_of_v<GameObject, ObjectType>, "is not gameObject");

	std::shared_ptr<ObjectType> newObject = std::make_shared<ObjectType>();
	std::shared_ptr<GameObject> baseObject = std::static_pointer_cast<GameObject>(newObject);
	baseObject->Name = name;
	baseObject->instanceID = instanceIDManager.getUniqueID();
	baseObject->myptr = baseObject;

	sceneManager.AddGameObject(baseObject);

	return newObject.get();
}