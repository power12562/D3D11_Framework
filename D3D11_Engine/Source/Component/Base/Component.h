#pragma once
#include <memory>

class Transform;
class GameObject;
class Component
{
	friend class GameObject;
public:
	Component();
	virtual ~Component();

private:
	void SetOwner(GameObject* gameObject);
	int index = -1;

private:
	GameObject* _gameObject = nullptr;

public:
	virtual void Start() = 0; //초기화 함수는 명시적으로 호출 가능.
protected:
	virtual void FixedUpdate() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;

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
