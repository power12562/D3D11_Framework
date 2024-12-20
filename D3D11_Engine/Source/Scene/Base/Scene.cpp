#include "Scene.h"
#include <Manager/SceneManager.h>
#include <D3DCore/D3DRenderer.h>
#include <Manager/HLSLManager.h>
#include <Manager/ResourceManager.h>
#include <D3DCore/D3DSamplerState.h>
#include <typeinfo>
#include <Utility/ImguiHelper.h>
#include <Utility/Console.h>
#include <Utility/SpinLock.h>
#include <GameObject/Base/GameObject.h>

Scene::Scene()
{
	constexpr unsigned int ReserveSize = 100000;
	sceneResourceList.reserve(ReserveSize);
	objectList.reserve(ReserveSize);
	dontdestroyonloadList.reserve(ReserveSize);
	d3dRenderer.reserveRenderQueue(ReserveSize);
}

Scene::~Scene()
{
	hlslManager.ClearSharingShader();
	Resource::ClearResourceManagers();
	D3DSamplerState::ClearSamplerResources();
}

void Scene::FixedUpdate()
{
	for (auto& obj : objectList)
	{
		if (obj && obj->Active)
			obj->FixedUpdate();
	}
}

void Scene::Update()
{
	for (auto& obj : objectList)
	{
		if (obj && obj->CheckActive())
		{
			obj->checkActive = obj->Active;
			if (obj->transform.GetChildCount())
				obj->UpdateChildActive(&obj->transform);
		}

		if (obj && obj->Active)
			obj->Update();
	}
}

void Scene::LateUpdate()
{
	for (auto& obj : objectList)
	{
		if (obj && obj->Active)
			obj->LateUpdate();
	}
}

void Scene::UpdateTransform()
{
	for (auto& obj : objectList)
	{
		if (obj && obj->Active)
		{
			obj->transform.UpdateTransform();
		}
	}
}

void Scene::Render()
{
	d3dRenderer.BegineDraw();	
	for (auto& obj : objectList)
	{
		if (obj && obj->Active)
			obj->Render();
	}
	d3dRenderer.EndDraw();
	if (UseImGUI && !objectList.empty())
	{
		ImGUIBegineDraw();
		ImGUIRender();
		ImGUIEndDraw();
	}
	d3dRenderer.Present();
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
	ImGui::ResetGlobalID();
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

void Scene::ClearResouceObj()
{
	sceneResourceList.clear();
}

