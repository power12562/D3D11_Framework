#pragma once
#include <memory>

class Transform;
class GameObject;
class Component
{
	friend class GameObject;
public:
	Component();
	~Component();

private:
	void SetOwner(GameObject* gameObject);
	int index = -1;

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

	Transform& GetTransform();
	__declspec (property(get = GetTransform)) Transform& transform;

	template <typename T>
	T& AddComponent();

	template <typename T>
	T& GetComponent();

	int GetComponentIndex() const { return index; }
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
