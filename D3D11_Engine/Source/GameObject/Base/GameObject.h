#pragma once
#include <Framework\Transform.h>
#include <Component\Base\Component.h>
#include <memory>

class GameObject
{		 
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

public:
	template <typename T>
	T& AddComponent();

	template <typename T>
	T& GetComponent();
};

template<typename T>
inline T& GameObject::AddComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "is not Component");

	T* nComponent = new T;
	nComponent->SetOwner(this);
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
			return *component;
		}
	}
	__debugbreak(); //예외) 존재하지 않는 컴포넌트
}
