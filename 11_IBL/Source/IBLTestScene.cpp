#include "IBLTestScene.h"
#include <framework.h>

IBLTestScene::IBLTestScene()
{
	UseImGUI = true;

	auto mainCam = NewGameObject<CameraObject>(L"mainCam");
	mainCam->transform.position += Vector3(0.f, 9.f, -20.f);
	mainCam->SetMainCamera();
	pCamera = Camera::GetMainCamera();
	pCameraMoveHelper = &mainCam->AddComponent<CameraMoveHelper>();

	skyBox = NewGameObject<SkyBoxObject>(L"SkyBox");
	skyBox->skyBoxRender.SetSkyBox(L"Resource/Skybox/TestIBLEnvHDR.dds");

	auto pistol = NewGameObject(L"pistol");
	auto initMesh = [this](MeshRender* mesh)
	{
			PBRMeshObject& obj = static_cast<PBRMeshObject&>(mesh->gameObject);
			obj.Material.UseRMACMap = true;
			mesh->textures.SetTexture2D(E_TEXTURE::RMACTexture, L"Resource/pistol/Cerberus_RMAC.dds");
	};
	Utility::LoadFBX(L"Resource/pistol/pistol.fbx", *pistol, initMesh, false, SURFACE_TYPE::PBR);
	pistol->transform.position = Vector3(-20.f, 0.f, 0.f);
	pistol->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };
	pistol->transform.rotation = Vector3{ 0.f, 90.f, 0.f };
	
	auto chara = NewGameObject(L"charater");
	Utility::LoadFBX(L"Resource/char/char.fbx", *chara, false, SURFACE_TYPE::PBR);
	chara->transform.position = Vector3(10.f, 0.f, 0.f);
	chara->transform.scale = Vector3(0.1f, 0.1f, 0.1f);
	chara->transform.rotation = Vector3(0.f, 25.f, 0.f);
}

IBLTestScene::~IBLTestScene()
{
}

void IBLTestScene::ImGUIRender()
{
	using namespace ImGui;
	using namespace PBRDirectionalLight;
	Begin("Debug");
	{
		if (ImGui::Button("Recompile Shader"))
		{
			MeshRender::ReloadShaderAll();
		}

		ImGui::Checkbox("Use Sky Box", &skyBox->Active);

		EditCamera("Main Camera", pCamera, pCameraMoveHelper);
		
		Text("Light");
		SliderFloat3("Light Dir", reinterpret_cast<float*>(&cb_light.LightDir), -1.f, 1.f);
		ColorEdit3("Light Diffuse", &cb_light.LightColor);
		ColorEdit3("Light Ambient", &cb_light.LightAmbient);
		DragFloat("Light Intensity", &cb_light.LightIntensity, 1.0f, 0.f, 100.f);
		Text("");

		ImGui::ColorEdit3("BackGround Color", &d3dRenderer.backgroundColor);
		ImGui::Text("");
	}
	End();
}
