#include "Scene.h"
#include <Framework\SceneManager.h>
#include <Framework/D3DRenderer.h>
#include <typeinfo>

Scene::Scene()
{
	
}

Scene::~Scene()
{

}

void Scene::FixedUpdate()
{
	for (auto& obj : objectList)
	{
		if (obj->Active)
			obj->FixedUpdate();
	}
}

void Scene::Update()
{
	for (auto& obj : objectList)
	{
		if (obj->Active)
			obj->Update();
	}
}

void Scene::LateUpdate()
{
	for (auto& obj : objectList)
	{
		if (obj->Active)
			obj->LateUpdate();
	}
}

void Scene::UpdateTransform()
{
	for (auto& obj : objectList)
	{
		if (obj->Active)
			obj->transform.UpdateTransform();
	}
}

void Scene::Render()
{
	d3dRenderer.BegineDraw();

	for (auto& obj : objectList)
	{
		if (obj->Active)
			obj->Render();
	}

	d3dRenderer.EndDraw();
}



