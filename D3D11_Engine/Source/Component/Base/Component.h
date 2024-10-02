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

};
