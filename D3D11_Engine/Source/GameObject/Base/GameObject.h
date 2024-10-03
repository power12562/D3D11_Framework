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
	/*컴포넌트 추가*/
	template <typename T>
	T& AddComponent();

	/*컴포넌트 가져오기*/
	template <typename T>
	T& GetComponent();

	/*이 오브젝트의 컴포넌트 개수*/
	int GetComponentCount() { return componentList.size(); }

	/*인덱스로 컴포넌트 가져오기. 파라미터로 캐스팅할 컴포넌트 타입을 전달.*/
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
	__debugbreak(); //예외) 존재하지 않는 컴포넌트
}

template<typename T>
inline T& GameObject::GetComponentAtIndex(int index)
{
	static_assert(std::is_base_of_v<Component, T>, "is not Component");

	T& component = dynamic_cast<T&>(*componentList[index]);

	return component;
}
