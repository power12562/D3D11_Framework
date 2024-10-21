#pragma once
#include <Framework\Transform.h>
#include <_Debug/Console.h>
#include <memory>
#include <string>

#pragma warning( disable : 4267)

class Component;
class RenderComponent;
class GameObject
{		 
	friend class Scene;
	friend class SceneManager;
public:
	static void Destroy(GameObject& obj);
	static void Destroy(GameObject* obj);
	static GameObject* Find(const wchar_t* name);

public:
	GameObject(); 
	virtual ~GameObject();

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
public:
	unsigned int GetInstanceID() const { return instanceID; }
	const std::wstring& GetName() const { return name; }
	const std::wstring& SetName(const wchar_t* _name);
public:
	/*������Ʈ �̸�. (�ߺ� ����)*/
	_declspec (property(get = GetName, put = SetName)) std::wstring& Name;
	Transform transform;
	bool Active = true;

public:
	/*������Ʈ �߰�*/
	template <typename T>
	T& AddComponent();

	/*������Ʈ ��������*/
	template <typename T>
	T& GetComponent();

	/*������Ʈ ��������. (������ nullptr ��ȯ)*/
	template <typename T>
	T* IsComponent();

	/*�ε����� ������Ʈ ��������. �Ķ���ͷ� ĳ������ ������Ʈ Ÿ���� ����.*/
	template <typename T>
	T* GetComponentAtIndex(int index);
	
	/*�� ������Ʈ�� ������Ʈ ����*/
	int GetComponentCount() { return componentList.size(); }
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

