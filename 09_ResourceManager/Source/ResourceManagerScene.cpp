#include "ResourceManagerScene.h"
#include "Utility/Console.h"
#include <Framework\SceneManager.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <Framework/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <Framework\ImguiHelper.h>
#include <Math\Mathf.h>
#include <Component\TransformAnimation.h>
#include <Light\SimpleDirectionalLight.h>
#include <Utility\AssimpUtility.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Framework\ResourceManager.h>
#include <Material/BlingPhongMaterial.h>
#include <Framework/TimeSystem.h>

#pragma warning(disable : 4305)
ResourceManagerScene::ResourceManagerScene()
{
	UseImGUI = true;

	//리소스 미리 로드
	Utility::LoadFBXResource(L"Resource/Hip Hop Dancing.fbx");

	SimpleDirectionalLight::cb_light.LightDir = { 0.5, 0, 1, 0 };

	d3dRenderer.backgroundColor = Color(0, 0, 0, 1);

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0, 30, -25);
	cam->transform.rotation = Vector3(-15, 0, 0);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;
	
	material = GetResourceManager<cb_BlingPhongMaterial>().GetResource(L"BlingPhong");

	for (size_t i = 0; i < 50; i++) { AddTestObject(); }
}

ResourceManagerScene::~ResourceManagerScene()
{
}

void ResourceManagerScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	cb_Light& cb_light = SimpleDirectionalLight::cb_light;

	ImGui::Begin("Debug");
	{
		ImGui::Text("Obj Count : %zu  FPS : %d", testList.size(), TimeSystem::Time.GetFrameRate());
		SYSTEM_VRAM_INFO vram = d3dRenderer.GetSystemVramInfo();
		ImGui::Text("Dedicated Video Memory : %zu", vram.DedicatedVideoMemory);
		ImGui::Text("Shared System Memory : %zu", vram.SharedSystemMemory);
		USAGE_VRAM_INFO local = d3dRenderer.GetLocalVramUsage();
		ImGui::Text("Local Video Memory : %llu MB / %llu MB", local.Budget, local.Usage);
		USAGE_VRAM_INFO nonlocal = d3dRenderer.GetNonLocalVramUsage();
		ImGui::Text("NonLocal Video Memory : %llu MB / %llu MB", nonlocal.Budget, nonlocal.Usage);

		SYSTEM_MEMORY_INFO memory = d3dRenderer.GetSystemMemoryInfo();
		ImGui::Text("PrivateUsage : %zu", memory.PrivateUsage);
		ImGui::Text("WorkingSetSize : %zu", memory.WorkingSetSize);
		ImGui::Text("PagefileUsage : %zu", memory.PagefileUsage);
		ImGui::Text("");

		ImGui::Text("Object");
		if (ImGui::Button("Add Charater"))
			AddTestObject();
		if (ImGui::Button("Sub Charater"))
			SubTestObject();
		if (ImGui::Button("Clear Charater"))
			ClearTestObject();

		ImGui::Text("Resource Manager");
		if (ImGui::Button("Clear Resource"))
			ClearAllResource();

		if (mainCam)
		{
			ImGui::Text("Camera");
			ImGui::SliderFloat("FOV", &mainCam->FOV, 10, 120);
			ImGui::SliderFloat("CamSpeed", pCamSpeed, 1, 1000);
			ImGui::DragVector3("Cam Position", &mainCam->transform.position, 0);
			ImGui::DragQuaternion("Cam Rotation", &mainCam->transform.rotation, 0);
			ImGui::Text("");
		}

		ImGui::Text("Light");
		ImGui::DragFloat3("LightDir", (float*)&cb_light.LightDir, 0.01f, -1.0f, 1.0f);
		ImGui::ColorEdit3("LightDiffuse", &cb_light.LightDiffuse);
		ImGui::ColorEdit3("LightAmbient", &cb_light.LightAmbient);
		ImGui::ColorEdit3("LightSpecular", &cb_light.LightSpecular);
		ImGui::Text("");

		ImGui::Text("Background");
		ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	}
	ImGui::End();
}
void ResourceManagerScene::AddTestObject()
{
	auto obj = NewGameObject<GameObject>(L"test");
	if (!testList.empty())
	{
		obj->transform.position = testList.back()->transform.position;
		if (std::abs(obj->transform.position.x - 30.f) <= Mathf::Epsilon)
		{
			obj->transform.position += Vector3::Left * 60;
			obj->transform.position += Vector3::Backward * 10;
		}
		else
		{
			obj->transform.position += Vector3::Right * 10;
		}
	}
	else
	{
		obj->transform.position = Vector3(-30, 0, 0);
	}
	obj->transform.scale = { 0.1,0.1,0.1 };

	auto testInit = [this](MeshRender* mesh)->void
		{
			int index = mesh->constBuffer.CreatePSConstantBuffers<cb_BlingPhongMaterial>();
			mesh->constBuffer.BindUpdateEvent(*material);

			index = mesh->constBuffer.CreatePSConstantBuffers<cb_Light>();
			mesh->constBuffer.BindUpdateEvent(SimpleDirectionalLight::cb_light);

			mesh->SetVertexShader(L"VertexSkinningShader.hlsl");
			mesh->SetPixelShader(L"PixelShader.hlsl");
		};
	Utility::LoadFBX(L"Resource/Hip Hop Dancing.fbx", *obj, testInit, false);
	obj->GetComponent<TransformAnimation>().PlayClip(L"mixamo.com");

	testList.push_back(obj);
}
void ResourceManagerScene::SubTestObject()
{
	if (testList.size() == 0)
		return;

	GameObject* obj = testList.back();
	sceneManager.DestroyObject(obj);
	testList.erase(--testList.end());
}
void ResourceManagerScene::ClearTestObject()
{
	while (!testList.empty())
	{
		SubTestObject();
	}
}
void ResourceManagerScene::ClearAllResource()
{
	ClearTestObject();
	ClearResouceObj();	//이 씬의 리소스 정리
	Resource::ClearResourceManagers(); //리소스 매니져 리소스 정리
}
#pragma warning(default : 4305)