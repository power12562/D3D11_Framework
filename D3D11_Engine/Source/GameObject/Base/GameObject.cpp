#include "GameObject.h"	   
#include <_Debug/Console.h>

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
	cb_Transform cb_transform;
	cb_transform.World = XMMatrixTranspose(transform.GetWM());
	cb_transform.WorldInverseTranspose = XMMatrixInverse(nullptr,transform.GetWM());
	cb_transform.WVP = XMMatrixTranspose(transform.GetWM() * Camera::GetMainCamera()->GetVM() * Camera::GetMainCamera()->GetPM());
	d3dRenderer.UpdateVSPSConstBuffer(cb_transform);

	D3DConstBuffer::UpdateStaticCbuffer(cb_transform);
	
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
