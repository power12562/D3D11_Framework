#include "Scene.h"
#include <Framework\SceneManager.h>
#include <Framework/D3DRenderer.h>
#include <typeinfo>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <_Debug/Console.h>

Scene::Scene()
{
	sceneResourceList.reserve(10);
	objectList.reserve(10);
	objectFindMap.reserve(10);
}

Scene::~Scene()
{
	
}

void Scene::FixedUpdate()
{
	for (auto& obj : objectList)
	{
		if (obj)
			if (obj->Active)
				obj->FixedUpdate();
	}
}

void Scene::Update()
{
	for (auto& obj : objectList)
	{
		if (obj)
			if (obj->Active)
				obj->Update();
	}
}

void Scene::LateUpdate()
{
	for (auto& obj : objectList)
	{
		if (obj)
			if (obj->Active)
				obj->LateUpdate();
	}
}

void Scene::UpdateTransform()
{
	for (auto& obj : objectList)
	{
		if (obj)
			if (obj->Active)
				obj->transform.UpdateTransform();
	}
}

void Scene::Render()
{
	d3dRenderer.BegineDraw();	
	for (auto& obj : objectList)
	{
		if (obj)
			if (obj->Active)
				obj->Render();
	}
	if (UseImGUI) 
	{
		ImGUIBegineDraw();
		ImGUIRender();
		ImGUIEndDraw();
	}
	d3dRenderer.EndDraw();
}

void Scene::ImGUIBegineDraw()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Scene::ImGUIEndDraw()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Scene::SetResouceObj(const wchar_t* key, GameObject* obj)
{
	sceneResourceList[key].emplace_back(obj->GetWeakPtr().lock());
	unsigned int childCount = obj->transform.GetChildCount();
	if (childCount > 0)
	{
		for (unsigned int i = 0; i < childCount; i++)
		{
			SetResouceObj(key, &obj->transform.GetChild(i)->gameObject);
		}
	}
}

void Scene::RemoveResouceObj(const wchar_t* key)
{
	auto findIter = sceneResourceList.find(key);
	if (findIter != sceneResourceList.end())
	{
		findIter->second.clear();
	}
	else
	{
		__debugbreak();
		Debug_printf("Resource not found.\n");
	}
}

