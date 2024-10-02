#pragma once
#include <Framework\Transform.h>
#include <memory>

class Component;
class GameObject
{		 
protected:
	GameObject();
	virtual ~GameObject();

private:
	void FixedUpdate();
	void Update();
	void LateUpdate();

	std::vector<std::unique_ptr<Component>> componentList;
public:
	Transform transform;
};