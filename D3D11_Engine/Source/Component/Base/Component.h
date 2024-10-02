#pragma once
#include <memory>
class GameObject;

class Component
{
	friend class GameObject;
public:
	Component(GameObject* gameObject);
	~Component();
protected:
	GameObject* _gameObject;

	virtual void Start();
	virtual void FixedUpdate();
	virtual void Update();
	virtual void LateUpdate();

public:
	bool Enable = true;

public:
	GameObject& GetGameObject();
	_declspec (property(get = GetGameObject)) GameObject& gameObject;

};
