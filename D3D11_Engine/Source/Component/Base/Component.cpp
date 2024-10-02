#include "Component.h"
#include <GameObject\Base\GameObject.h>

void Component::SetOwner(GameObject* gameObject)
{
	this->_gameObject = gameObject;	
}

Component::Component()
{
	
}

Component::~Component()
{
}

void Component::Start()
{
}

void Component::FixedUpdate()
{
}

void Component::Update()
{
}

void Component::LateUpdate()
{
}

GameObject& Component::GetGameObject()
{
	return *_gameObject;
}
