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
	GameObject();
	/*�̸�, �ν��Ͻ� ���̵� �ο� �� ȣ��Ǵ� �Լ�.*/
	virtual void Start() {};
	virtual ~GameObject();

	/** �߰������� ����ȭ�� ������ �ʿ�� �������̵�*/
	virtual void Serialized(std::ofstream& ofs) {};
	/** �߰������� ����ȭ�� ������ �ʿ�� �������̵�*/
	virtual void Deserialized(std::ifstream& ifs) {};
public:
	static void Destroy(GameObject& obj);
	static void Destroy(GameObject* obj);
	static void DontDestroyOnLoad(GameObject& obj);
	static void DontDestroyOnLoad(GameObject* obj);
	static GameObject* Find(const wchar_t* name);
	template<typename ObjectType>
	inline static ObjectType* NewGameObject(const wchar_t* name);

public:
	unsigned int GetInstanceID() const { return instanceID; }
	const std::wstring& GetName() const { return name; }
	const std::wstring& SetName(const wchar_t* _name);
	std::string GetNameToString() const;
private:
	std::wstring name;
	unsigned int instanceID = -1;

public:
	Transform transform;
	bool Active = true;

public:
	/*������Ʈ �̸�. (�ߺ� ����)*/
	_declspec (property(get = GetName, put = SetName)) std::wstring& Name;

	/*������Ʈ �߰�*/
	template <typename T>
	T& AddComponent();

	/*������Ʈ ��������*/
	template <typename T>
	T& GetComponent();

	/*������Ʈ ��������. (������ nullptr ��ȯ)*/
	template <typename T>
	T* IsComponent();

	/*������Ʈ �ּҷ� �ε��� Ȯ���ϱ�. ������ -1 ��ȯ*/
	int GetComponentIndex(Component* findComponent);

	/*�ε����� ������Ʈ ��������. �Ķ���ͷ� ĳ������ ������Ʈ Ÿ���� ����.*/
	template <typename T>
	T* GetComponentAtIndex(int index);

	/*�� ������Ʈ�� ������Ʈ ����*/
	int GetComponentCount() { return componentList.size(); }

	void DestroyComponent(Component& component);

	void DestroyComponent(Component* component);

	DirectX::BoundingOrientedBox GetOBBToWorld() const;
	DirectX::BoundingBox GetBBToWorld() const;

	//������Ʈ Bounds �ʱ� center ���� Mathf::FLOAT_MAX�� �ʱ�ȭ �˴ϴ�.
	DirectX::BoundingBox Bounds;

	/*ī�޶� �ø� ����*/
	inline bool IsCameraCulling() const { return isCulling; }
private:
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void Render();
private:
	std::vector<std::unique_ptr<Component>> componentList;
	std::vector<RenderComponent*> renderList;
public:
	std::weak_ptr<GameObject> GetWeakPtr() { return myptr; }
private:
	std::weak_ptr<GameObject> myptr;
	bool isCulling = false; //�̹� ������ ī�޶� �ø� ����
	bool checkActive = true;
	inline bool CheckActive() const { return checkActive != Active; }
	void UpdateChildActive(Transform* rootTransform);
};

template<typename T>
inline T& GameObject::AddComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "is not Component");

	T* nComponent = new T;
	nComponent->SetOwner(this);
	nComponent->index = componentList.size();
	nComponent->Start();
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
	__debugbreak(); //����) �������� �ʴ� ������Ʈ
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

	unsigned int id = instanceIDManager.getUniqueID();
	void* pointer = gameObjectFactory.GameObjectAlloc(id);
	if (pointer)
	{
		ObjectType* pObject = reinterpret_cast<ObjectType*>(pointer);
		std::shared_ptr<ObjectType> newObject(pObject, GameObjectFactory::GameObjectDeleter);
		std::shared_ptr<GameObject> baseObject = std::static_pointer_cast<GameObject>(newObject);

		new(pObject)ObjectType();
		baseObject->Name = name;
		baseObject->instanceID = id;
		baseObject->myptr = baseObject;
		baseObject->Start();

		sceneManager.AddGameObject(baseObject);
		return newObject.get();
	}
	else
	{
		__debugbreak();
		return nullptr;
	}
}