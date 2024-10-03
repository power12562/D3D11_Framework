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

GameObject& Component::GetGameObject()
{
	return *_gameObject;
}

Transform& Component::GetTransform()
{
	return gameObject.transform;
}
