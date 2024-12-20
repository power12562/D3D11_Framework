#include "GameObject.h"	   
#include <Utility/Console.h>

#include <Component/Camera/Camera.h>
#include <Component/Base/RenderComponent.h>
#include <D3DCore/D3DRenderer.h>
#include <Manager/SceneManager.h>
#include <Manager/InstanceIDManager.h>
#include <D3DCore/D3DConstBuffer.h>
#include <Utility/utfConvert.h>

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

DirectX::BoundingOrientedBox GameObject::GetOBBToWorld() const
{
	BoundingOrientedBox boundingBoxOut;
	boundingBoxOut.CreateFromBoundingBox(boundingBoxOut, BoundingBox);
	boundingBoxOut.Transform(boundingBoxOut, transform.GetWM());
	return boundingBoxOut;
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

void GameObject::UpdateChildActive(Transform* rootObject)
{
	for (auto& child : rootObject->childList)
	{
		child->gameObject.Active = rootObject->gameObject.Active;
		child->gameObject.checkActive = rootObject->gameObject.checkActive;
		if (!child->childList.empty())
			UpdateChildActive(child);
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

std::string GameObject::GetNameToString() const
{
	return utfConvert::wstring_to_utf8(GetName());
}