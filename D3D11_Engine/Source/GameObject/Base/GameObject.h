#pragma once
#include <Framework\Transform.h>
#include <memory>

class Component;
class GameObject
{		 
	friend class Scene;
public:
	GameObject();
	virtual ~GameObject();

private:
	void FixedUpdate();
	void Update();
	void LateUpdate();

	std::vector<std::unique_ptr<Component>> componentList;
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
	componentList.emplace_back(nComponent);	
	nComponent->index = componentList.size() - 1;
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
			return *component;
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
