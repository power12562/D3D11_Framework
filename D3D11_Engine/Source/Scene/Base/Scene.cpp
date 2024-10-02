#include "Scene.h"
#include <Framework\SceneManager.h>
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



