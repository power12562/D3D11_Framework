#include "GameObject.h"	   
#include <Utility/Console.h>

#include <Component/Camera/Camera.h>
#include <Component\Base\RenderComponent.h>
#include <Framework/D3DRenderer.h>
#include <Framework/SceneManager.h>
#include <Framework/InstanceIDManager.h>
#include <Framework/D3DConstBuffer.h>

void GameObject::Destroy(GameObject& obj)
{
	sceneManager.DestroyObject(obj);
}

void GameObject::Destroy(GameObject* obj)
{
	sceneManager.DestroyObject(obj);
}

GameObject* GameObject::Find(const wchar_t* name)
{
	return sceneManager.FindObject(name);
}

GameObject::GameObject()
{
	transform._gameObject = this;
}

GameObject::~GameObject()
{
	//Debug_printf("%s, destroy\n", Name);
	instanceIDManager.returnID(instanceID);
}

void GameObject::DestroyComponent(Component& component)
{
	DestroyComponent(&component);
}

void GameObject::DestroyComponent(Component* component)
{
	int size = (int)componentList.size();
	int comIndex = 0;
	for (auto& com : componentList)
	{
		if (typeid(*com) == typeid(*component))
			break;

		++comIndex;
	}
	int renderIndex = 0;
	if (comIndex < size)
	{
		size = renderList.size();
		for (auto& com : renderList)
		{
			if (typeid(*com) == typeid(*component))
				break;

			renderIndex++;
		}
		componentList.erase((componentList.begin() + comIndex));
		if (renderIndex < size)
			renderList.erase((renderList.begin() + renderIndex));
	}
}

void GameObject::FixedUpdate()
{
	for (auto& component : componentList)
	{
		if(component->Enable)
			component->FixedUpdate();
	}
}

void GameObject::Update()
{
	for (auto& component : componentList)
	{
		if (component->Enable)
			component->Update();
	}
}

void GameObject::LateUpdate()
{
	for (auto& component : componentList)
	{
		if (component->Enable)
			component->LateUpdate();
	}
}

void GameObject::Render()
{	
	for (auto& component : renderList)
	{
		if (component->Enable)
			component->Render();
	}
}

const std::wstring& GameObject::SetName(const wchar_t* _name)
{
	if (wcscmp(name.c_str(), _name) == 0)
		return name;

	sceneManager.ChangeObjectName(this->instanceID, name, _name);
	name = _name;
	return name;
}
