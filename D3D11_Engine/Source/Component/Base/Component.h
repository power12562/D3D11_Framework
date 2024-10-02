#pragma once
#include <memory>

class GameObject;
class Component
{
	friend class GameObject;
public:
	Component();
	~Component();

private:
	void SetOwner(GameObject* gameObject);

protected:
	GameObject* _gameObject = nullptr;

	virtual void Start();
	virtual void FixedUpdate();
	virtual void Update();
	virtual void LateUpdate();

public:
	bool Enable = true;

public:
	GameObject& GetGameObject();
	_declspec (property(get = GetGameObject)) GameObject& gameObject;

	template <typename T>
	T& AddComponent();

	template <typename T>
	T& GetComponent();
};

#include <GameObject\Base\GameObject.h>
template<typename T>
inline T& Component::AddComponent()
{
	return gameObject.AddComponent<T>();
}

template<typename T>
inline T& Component::GetComponent()
{
	return gameObject.GetComponent<T>();
}
