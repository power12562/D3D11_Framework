#include "ShadowTestScene.h"
#include <framework.h>

constexpr float positionDumpX = 20;
constexpr float positionDumpZ = 20;
void ShadowTestScene::Start()
{
	Scene::UseImGUI = true;

	auto mainCam = NewGameObject<CameraObject>(L"mainCam");
	mainCam->transform.position += Vector3(0.f, 0.f, -20.f);
	mainCam->SetMainCamera();
	pCamera = Camera::GetMainCamera();
	pCameraMoveHelper = &mainCam->AddComponent<CameraMoveHelper>();

	skyBox = NewGameObject<SkyBoxObject>(L"SkyBox");
	skyBox->skyBoxRender.SetSkyBox(SkyBoxRender::ENV, L"Resource/Skybox/RoomEnvHDR.dds");
	skyBox->skyBoxRender.SetSkyBox(SkyBoxRender::Diffuse_IBL, L"Resource/Skybox/RoomDiffuseHDR.dds");
	skyBox->skyBoxRender.SetSkyBox(SkyBoxRender::Specular_IBL, L"Resource/Skybox/RoomSpecularHDR.dds");
	skyBox->skyBoxRender.SetSkyBox(SkyBoxRender::BRDF_LUT, L"Resource/Skybox/RoomBrdf.dds");
	
	cube = NewGameObject<CubeObject>(L"ground");

	constexpr int count = 1;
	size_t zCount = static_cast<size_t>(std::sqrt(count * 3));
	for (size_t i = 0; i < count; i++)
	{
		AddObjects(zCount);
	}
	size_t centerRow = zCount / 2;
	size_t centerColumn = zCount / 2;

	float centerX = centerColumn * positionDumpX;
	float centerZ = centerRow * positionDumpZ;

	cube->transform.position = Vector3(centerX, -5.f, centerZ);
	float cubeScale = (float)zCount;
	cube->transform.scale = Vector3(count * 5, 0.5f, count * 5);
}

void ShadowTestScene::ImGUIRender()
{
	static bool ShowSphereEdit = false;
	static bool ShowPistolEdit = false;
	static bool ShowCharEdit = false;

	ImGui::Begin("Debug");
	{
		ImGui::Text("%d", TimeSystem::Time.GetFrameRate());
		ImGui::Button("Show Sphere Edit", &ShowSphereEdit);
		ImGui::Button("Show Pistol Edit", &ShowPistolEdit);
		ImGui::Button("Show Char Edit", &ShowCharEdit);

		ImGui::ColorEdit3("BackGround Color", &d3dRenderer.backgroundColor);
		ImGui::Text("");
	}
	ImGui::End();

	ImGui::Begin("Light");
	{
		ImGui::EditLight(&DirectionalLight::DirectionalLights);
	}
	ImGui::End();

	if (ShowSphereEdit)
	{
		ImGui::Begin("Sphere Edit", &ShowSphereEdit);
		{
			ImGui::EditMaterial("Sphere Material", sphereMaterial);
		}
		ImGui::End();
	}

	if (ShowPistolEdit)
	{
		ImGui::Begin("Pistol Edit", &ShowPistolEdit);
		{
			for (auto& [key, Material] : pistolMaterials)
			{
				ImGui::EditMaterial(key.c_str(), Material);
			}
		}
		ImGui::End();
	}

	if (ShowCharEdit)
	{
		ImGui::Begin("Char Edit", &ShowCharEdit);
		{
			for (auto& [key, obj] : charMaterials)
			{
				ImGui::Checkbox(key.c_str(), &obj->Active);
				ImGui::EditMaterial(key.c_str(), &obj->Material);
			}
		}
		ImGui::End();
	}

}

void ShadowTestScene::AddObjects(size_t positionZcount)
{
	static size_t positionCountX = 0;
	static size_t positionCountZ = 1;

	auto pistol = NewGameObject(L"pistol");
	auto initMesh = [this](MeshRender* mesh)
		{
			PBRMeshObject& obj = static_cast<PBRMeshObject&>(mesh->gameObject);
			obj.Material.UseRMACMap = true;
			std::string key = obj.GetNameToString();
			if (pistolMaterials.find(key) == pistolMaterials.end())
				pistolMaterials[key] = &obj.Material;
			else
			{
				cb_PBRMaterial& material = *(pistolMaterials[key]);
				mesh->constBuffer.BindUpdateEvent(material);
			}
			mesh->textures.SetTexture2D(E_TEXTURE::RMACTexture, L"Resource/pistol/Cerberus_RMAC.dds");
		};
	Utility::LoadFBX(L"Resource/pistol/pistol.fbx", *pistol, initMesh, false, SURFACE_TYPE::PBR);
	pistol->transform.position = Vector3(positionCountX * positionDumpX, 0.f, positionCountZ * positionDumpZ);
	pistol->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };
	pistol->transform.rotation = Vector3{ 0.f, 90.f, 0.f };
	positionCountX++;
	if (positionZcount == positionCountX)
	{
		positionCountZ++;
		positionCountX = 0;
	}

	auto sphere = NewGameObject<SphereObject>(L"Sphere");
	{
		sphere->transform.position = Vector3(positionCountX * positionDumpX, 0.f, positionCountZ * positionDumpZ);
		if(sphereMaterial == nullptr)
			sphereMaterial = &sphere->Material;
		sphereMaterial->Albedo = { 1.f, 0.8453f, 0.f, 1.f };
		sphereMaterial->Metalness = 1.f;
		sphereMaterial->Roughness = 0.f;
		sphere->sphereMeshRender.CreateSphere(5.0f, 100, 100);;
		sphere->sphereMeshRender.constBuffer.BindUpdateEvent(*sphereMaterial);
	}
	positionCountX++;
	if (positionZcount == positionCountX)
	{
		positionCountZ++;
		positionCountX = 0;
	}

	auto cha = NewGameObject(L"ch");
	auto charInit = [this](MeshRender* mesh)
		{
			PBRMeshObject* obj = static_cast<PBRMeshObject*>(&mesh->gameObject);
			std::string key = obj->GetNameToString();
			if (charMaterials.find(key) == charMaterials.end())
				charMaterials[key] = obj;
			else
			{
				cb_PBRMaterial& material = charMaterials[key]->Material;
				mesh->constBuffer.BindUpdateEvent(material);
			}			
		};
	Utility::LoadFBX(L"Resource/char/char.fbx", *cha, charInit, false, SURFACE_TYPE::PBR);
	cha->transform.position = Vector3(positionCountX * positionDumpX, 0.f, positionCountZ * positionDumpZ);
	cha->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };
	cha->transform.rotation = Vector3::Up * 23.f;

	positionCountX++;
	if (positionZcount == positionCountX)
	{
		positionCountZ++;
		positionCountX = 0;
	}	
}