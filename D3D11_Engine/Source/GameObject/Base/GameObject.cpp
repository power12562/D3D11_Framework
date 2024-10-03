#include "GameObject.h"	   
#include <_Debug/Console.h>
#include <Component/Camera.h>
#include <Framework/D3DRenderer.h>
#include <Framework/SceneManager.h>

GameObject::GameObject(const wchar_t* name)
{
	this->name = name;
	sceneManager.objectAddQueue.push(this);
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
	cbuffer_Transform cb_T;
	cb_T.World = XMMatrixTranspose(transform.GetWM());
	cb_T.View = XMMatrixTranspose(Camera::GetMainCamera()->GetVM());
	cb_T.Projection = XMMatrixTranspose(Camera::GetMainCamera()->GetPM());
	cb_T.WVP = XMMatrixTranspose(transform.GetWM() * Camera::GetMainCamera()->GetVM() * Camera::GetMainCamera()->GetPM());
	d3dRenderer.UpdateConstBuffer(cb_T);

	for (auto& component : componentList)
	{
		if (component->Enable)
			component->Render();
	}
}
