#include "Component.h"
#include <GameObject\Base\GameObject.h>
#include <_Debug\Console.h>

void Component::SetOwner(GameObject* gameObject)
{
	this->_gameObject = gameObject;	
}

Component::Component()
{
	
}

Component::~Component()
{
	//Debug_printf("ÄÄÆ÷³ÍÆ® ¼Ò¸ê\n");
}

GameObject& Component::GetGameObject()
{
	return *_gameObject;
}

Transform& Component::GetTransform()
{
	return gameObject.transform;
}
