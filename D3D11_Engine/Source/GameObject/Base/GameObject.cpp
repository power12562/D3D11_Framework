#include "GameObject.h"	   
#include <_Debug/Console.h>

#include <Component/Camera.h>
#include <Component\Base\RenderComponent.h>
#include <Framework/D3DRenderer.h>
#include <Framework/SceneManager.h>

GameObject::GameObject(const wchar_t* name)
{
	this->name = name;
	if (sceneManager.nextScene)
		sceneManager.nextAddQueue.push(this);
	else
		sceneManager.currAddQueue.push(this);
}

GameObject::~GameObject()
{
	//Debug_printf("%s, destroy\n", name);
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
