#include "DeferredTestScene1.h"
#include <framework.h>

void DeferredTestScene1::Start()
{
	UseImGUI = true;
	GuizmoSetting.UseImGuizmo = true;

	pSkyBox = NewGameObject<SkyBoxObject>(L"SkyBox");
	pSkyBox->skyBoxRender.SetSkyBox(SkyBoxRender::ENV, L"Resource/Skybox/RoomEnvHDR.dds");
	pSkyBox->skyBoxRender.SetSkyBox(SkyBoxRender::Diffuse_IBL, L"Resource/Skybox/RoomDiffuseHDR.dds");
	pSkyBox->skyBoxRender.SetSkyBox(SkyBoxRender::Specular_IBL, L"Resource/Skybox/RoomSpecularHDR.dds");
	pSkyBox->skyBoxRender.SetSkyBox(SkyBoxRender::BRDF_LUT, L"Resource/Skybox/RoomBrdf.dds");

	auto mainCamera = NewGameObject<CameraObject>(L"MainCamera");
	mainCamera->SetMainCamera();
	mainCamera->transform.position = Vector3(-100, 95, -170);
	pCamera = Camera::GetMainCamera();
	pCameraMoveHelper = &mainCamera->AddComponent<CameraMoveHelper>();

	auto cube = NewGameObject<CubeObject>(L"Cube");
	cube->transform.scale = Vector3(1000.f, 0.1f, 1000.f);

	Sphere = NewGameObject<SphereObject>(L"Sphere");
	Sphere->transform.position = Vector3(-180.f, 65.f, 0.f);
	Sphere->transform.scale = 1.25f;

	auto chr = NewGameObject(L"chr");
	Utility::LoadFBX(L"Resource/char/char.fbx", *chr, false, SURFACE_TYPE::PBR);
	chr->transform.position += Vector3::Up * 10.f;
	chr->transform.rotation = Vector3(0, 30, 0);
}

void DeferredTestScene1::ImGUIRender()
{
	ImGui::Begin("Sphere");
	{
		ImGui::EditMaterial("Sphere Material", &Sphere->Material);
	}
	ImGui::End();

	ImGui::Begin("Renderer");
	{
		ImGui::EditD3DRenderer();
		ImGui::EditCamera("Main Camera", pCamera, pCameraMoveHelper);
	}
	ImGui::End();

	ImGui::Begin("Light");
	{
		ImGui::Checkbox("Active Sky Box", &pSkyBox->Active);
		ImGui::EditLight(&DirectionalLight::DirectionalLights);
	}
	ImGui::End();

	ImGui::Begin("Gbuffer");
	{
		for (int i = 0; i < 7; i++)
		{
			void* image = (void*)d3dRenderer.GetGbufferSRV(i);
			if (image)
			{
				ImGui::Text("Gbuffer%d", i);
				ImGui::Image(image, ImVec2(426, 240));
			}
			else
				break;
		}
	}
	ImGui::End();
}
