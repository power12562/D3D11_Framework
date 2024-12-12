#include "IBLTestScene.h"
#include <framework.h>

IBLTestScene::IBLTestScene()
{
	UseImGUI = true;
	d3dRenderer.backgroundColor = { 0.3f, 0.3f, 0.3f, 1.f };

	auto mainCam = NewGameObject<CameraObject>(L"mainCam");
	mainCam->transform.position += Vector3(0.f, 0.f, -20.f);
	mainCam->SetMainCamera();
	pCamera = Camera::GetMainCamera();
	pCameraMoveHelper = &mainCam->AddComponent<CameraMoveHelper>();

	skyBox = NewGameObject<SkyBoxObject>(L"SkyBox");
	skyBox->skyBoxRender.SetSkyBox(SkyBoxRender::ENV, L"Resource/Skybox/TestIBLEnvHDR.dds");
	skyBox->skyBoxRender.SetSkyBox(SkyBoxRender::Diffuse_IBL, L"Resource/Skybox/TestIBLDiffuseHDR.dds");
	skyBox->skyBoxRender.SetSkyBox(SkyBoxRender::Specular_IBL, L"Resource/Skybox/TestIBLSpecularHDR.dds");
	skyBox->skyBoxRender.SetSkyBox(SkyBoxRender::BRDF_LUT, L"Resource/Skybox/TestIBLBrdf.dds");

	pistol = NewGameObject(L"pistol");
	auto initMesh = [this](MeshRender* mesh)
	{
			PBRMeshObject& obj = static_cast<PBRMeshObject&>(mesh->gameObject);
			obj.Material.UseRMACMap = true;
			pistolMaterials[obj.GetNameToString()] = &obj.Material;
			mesh->textures.SetTexture2D(E_TEXTURE::RMACTexture, L"Resource/pistol/Cerberus_RMAC.dds");
	};
	Utility::LoadFBX(L"Resource/pistol/pistol.fbx", *pistol, initMesh, false, SURFACE_TYPE::PBR);
	pistol->transform.position = Vector3(-20.f, 0.f, 0.f);
	pistol->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };
	pistol->transform.rotation = Vector3{ 0.f, 90.f, 0.f };
	
	Sphere = NewGameObject<SphereObject>(L"Sphere");
	{
		sphereMaterial = &Sphere->Material;
		sphereMaterial->Albedo = { 1.f, 0.8453f, 0.f, 1.f };
		sphereMaterial->Metalness = 1.f;
		sphereMaterial->Roughness = 0.f;
		Sphere->sphereMeshRender.CreateSphere(5.0f, 100, 100);;
	}

	auto cha = NewGameObject(L"ch");
	Utility::LoadFBX(L"Resource/char/char.fbx", *cha, false, SURFACE_TYPE::PBR);
	cha->transform.position = Vector3(20.f, 0.f, 0.f);
	cha->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };
	cha->transform.rotation = Vector3::Up * 23.f;


}

IBLTestScene::~IBLTestScene()
{
}

void IBLTestScene::ImGUIRender()
{
	using namespace ImGui;
	using namespace PBRDirectionalLight;

	static bool ShowSphereEdit = false;
	static bool ShowPistolEdit = false;
	Begin("Debug");
	{
		if (ImGui::Button("Recompile Shader"))
		{
			MeshRender::ReloadShaderAll();
		}

		ImGui::Checkbox("Use Sky Box", &skyBox->Active);
		if (ImGui::Button("Show Sphere Edit"))
			ShowSphereEdit = !ShowSphereEdit;
		if (ImGui::Button("Show Pistol Edit"))
			ShowPistolEdit = !ShowPistolEdit;
		Text("");
		EditCamera("Main Camera", pCamera, pCameraMoveHelper);	
		ImGui::EditLight("Directional Light", &PBRDirectionalLight::cb_light);
		ImGui::ColorEdit3("BackGround Color", &d3dRenderer.backgroundColor);
		ImGui::Text("");
	}
	End();

	if (ShowSphereEdit)
	{
		Begin("Sphere Edit", &ShowSphereEdit);
		{
			EditTransform(Sphere);
			EditMaterial("Sphere Material", sphereMaterial);
		}
		End();
	}

	if (ShowPistolEdit)
	{
		Begin("Pistol Edit", &ShowPistolEdit);
		{
			EditTransform(pistol);
			for (auto& [key, Material] : pistolMaterials)
			{
				EditMaterial(key.c_str(), Material);
			}
		}
		End();
	}
}
