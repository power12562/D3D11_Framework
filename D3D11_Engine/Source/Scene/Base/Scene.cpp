#include "Scene.h"
#include <Framework\SceneManager.h>
#include <Framework/D3DRenderer.h>
#include <typeinfo>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

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
}

void Scene::ImGUIEndDraw()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

