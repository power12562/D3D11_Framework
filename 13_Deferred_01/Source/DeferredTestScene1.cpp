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
	cube->transform.position += Vector3::Down * 30.f;
	
	Sphere = NewGameObject<SphereObject>(L"Sphere");
	Sphere->transform.position = Vector3(-180.f, 65.f, 0.f);
	Sphere->transform.scale = 1.25f;

	constexpr int objCount = 100;
	int w = (int)std::sqrt(objCount);
	for (int y = 0; y < w; y++)
	{
		for (int x = 0; x < w; x++)
		{
			auto chr = NewGameObject(L"chr");
			auto func = [&](MeshRender* mesh)
				{
					chrMeshs.push_back(mesh);
				};
			Utility::LoadFBX(L"Resource/char/char.fbx", *chr, func, false, SURFACE_TYPE::PBR);
			chr->transform.position += Vector3::Up * 10.f;
			chr->transform.position += Vector3::Right * 50.f * (float)x;
			chr->transform.position += Vector3::Forward * 50.f * (float)y;
			chr->transform.rotation = Vector3(0, 30, 0);
		}
	}	
}

void DeferredTestScene1::ImGUIRender()
{
	ImGui::Begin("Sphere");
	{
		if (Sphere)
		{
			ImGui::EditMaterial("Sphere Material", &Sphere->Material);
		}		
	}
	ImGui::End();

	ImGui::Begin("Renderer");
	{
		static bool deferredFlag = true;
		ImGui::Checkbox("Deferred On / Off", &deferredFlag);
		if (deferredFlag != this->deferredFlag)
		{
			this->deferredFlag = deferredFlag;
			if (deferredFlag)
			{
				for (auto& i : chrMeshs)
				{
					if (!i->isAlpha)
					{
						i->isForward = false;
						i->SetPixelShader(L"Resource/EngineShader/PBROpaquePS.hlsl");
					}
				}
			}
			else
			{
				for (auto& i : chrMeshs)
				{
					if (!i->isAlpha)
					{
						i->isForward = true;
						i->SetPixelShader(L"Resource/EngineShader/PBRForwardPS.hlsl");
					}
				}
			}
		}
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
