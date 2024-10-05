#include "GameObject.h"	   
#include <_Debug/Console.h>

#include <Component/Camera.h>
#include <Component\Base\RenderComponent.h>
#include <Framework/D3DRenderer.h>
#include <Framework/SceneManager.h>
#include <Framework/InstanceIDManager.h>

void GameObject::Destroy(GameObject& obj)
{
	sceneManager.DestroyObject(obj);
}

void GameObject::Destroy(GameObject* obj)
{
	sceneManager.DestroyObject(obj);
}

GameObject::GameObject(const wchar_t* name)
{
	if (sceneManager.nextScene)
		sceneManager.nextAddQueue.push(this);
	else
		sceneManager.currAddQueue.push(this);

	this->Name = name;
	instanceID = instanceIDManager.getUniqueID();
}

GameObject::~GameObject()
{
	//Debug_printf("%s, destroy\n", Name);
	instanceIDManager.returnID(instanceID);
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
	cbuffer_Transform cb_Transform;
	cb_Transform.World = XMMatrixTranspose(transform.GetWM());
	cb_Transform.WorldInverseTranspose = XMMatrixInverse(nullptr,transform.GetWM());
	cb_Transform.WVP = XMMatrixTranspose(transform.GetWM() * Camera::GetMainCamera()->GetVM() * Camera::GetMainCamera()->GetPM());
	d3dRenderer.UpdateConstBuffer(cb_Transform);
	
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
