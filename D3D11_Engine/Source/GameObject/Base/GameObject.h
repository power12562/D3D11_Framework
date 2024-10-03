#pragma once
#include <Framework\Transform.h>
#include <memory>
#include <string>

#pragma warning( disable : 4267)

class Component;
class GameObject
{		 
	friend class Scene;
public:
	GameObject(const wchar_t* name);
	virtual ~GameObject();

private:
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void Render();

private:
	std::vector<std::unique_ptr<Component>> componentList;

private:
	std::wstring name;

public:
	Transform transform;
	bool Active = true;

public:
	/*������Ʈ �߰�*/
	template <typename T>
	T& AddComponent();

	/*������Ʈ ��������*/
	template <typename T>
	T& GetComponent();

	/*�� ������Ʈ�� ������Ʈ ����*/
	int GetComponentCount() { return componentList.size(); }

	/*�ε����� ������Ʈ ��������. �Ķ���ͷ� ĳ������ ������Ʈ Ÿ���� ����.*/
	template <typename T>
	T& GetComponentAtIndex(int index);
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
	return *nComponent;
}

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

template<typename T>
inline T& GameObject::GetComponentAtIndex(int index)
{
	static_assert(std::is_base_of_v<Component, T>, "is not Component");

	T& component = dynamic_cast<T&>(*componentList[index]);

	return component;
}
