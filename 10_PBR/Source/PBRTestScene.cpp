#include "PBRTestScene.h"
#include <Framework\SceneManager.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <Framework/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <Framework\ImguiHelper.h>
#include <Light\SimpleDirectionalLight.h>
#include <Utility\AssimpUtility.h>
#include <Material/SimpleMaterial.h>
#include <Framework/TimeSystem.h>
#include <Component/Render/MeshRender.h>

#pragma warning(disable : 4305)
PBRTestScene::PBRTestScene()
{
	UseImGUI = true;

	d3dRenderer.backgroundColor = Color(0, 0, 0, 1);

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(7.7f, 0.f, 13.6f);
	cam->transform.rotation = Vector3(10.176f, 124.5f, -14.43f);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	auto cerberus = NewGameObject(L"cerberus");
	auto initShader = [](MeshRender* mesh)
		{ 
			mesh->SetVertexShader(L"Shader/PBRVertexShader.hlsl");
			mesh->SetPixelShader(L"Shader/PBRPixelShader.hlsl");
		};
	Utility::LoadFBX(L"Resource/cerberus.fbx", *cerberus, nullptr, initShader, false);
	cerberus->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };
}

PBRTestScene::~PBRTestScene()
{
}

void PBRTestScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();

	ImGui::Begin("Debug");
	{
		ImGui::Text("FPS : %d", TimeSystem::Time.GetFrameRate());
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

		if (mainCam)
		{
			ImGui::Text("Camera");
			ImGui::SliderFloat("FOV", &mainCam->FOV, 10, 120);
			ImGui::SliderFloat("CamSpeed", pCamSpeed, 1, 1000);
			ImGui::DragVector3("Cam Position", &mainCam->transform.position, 0);
			ImGui::DragQuaternion("Cam Rotation", &mainCam->transform.rotation, 0);
			ImGui::Text("");
		}

		ImGui::Text("Background");
		ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	}
	ImGui::End();
}
