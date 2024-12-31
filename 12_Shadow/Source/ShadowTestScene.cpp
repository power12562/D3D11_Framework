#include "ShadowTestScene.h"
#include <framework.h>

constexpr float positionDumpX = 50;
constexpr float positionDumpZ = 50;
void ShadowTestScene::Start()
{
	positionCountX = 0;
	positionCountZ = 1;

	Scene::UseImGUI = true;
	GuizmoSetting.UseImGuizmo = true;
	d3dRenderer.DebugSetting.DrawObjectCullingBox = true;

	auto mainCam = NewGameObject<CameraObject>(L"mainCam");
	mainCam->transform.position += Vector3(0.f, 0.f, -20.f);
	mainCam->SetMainCamera();
	pCamera = Camera::GetMainCamera();
	pCameraMoveHelper = &mainCam->AddComponent<CameraMoveHelper>();
	pCamera->Far = 1000;

	skyBox = NewGameObject<SkyBoxObject>(L"SkyBox");
	skyBox->skyBoxRender.SetSkyBox(SkyBoxRender::ENV, L"Resource/Skybox/RoomEnvHDR.dds");
	skyBox->skyBoxRender.SetSkyBox(SkyBoxRender::Diffuse_IBL, L"Resource/Skybox/RoomDiffuseHDR.dds");
	skyBox->skyBoxRender.SetSkyBox(SkyBoxRender::Specular_IBL, L"Resource/Skybox/RoomSpecularHDR.dds");
	skyBox->skyBoxRender.SetSkyBox(SkyBoxRender::BRDF_LUT, L"Resource/Skybox/RoomBrdf.dds");
	
	cube = NewGameObject<CubeObject>(L"ground");
	cube->transform.scale = Vector3(1000.f, 0.5f, 1000.f);

	constexpr int count = 100;
	size_t zCount = static_cast<size_t>(std::sqrt(count * 4));
	for (size_t i = 0; i < count; i++)
	{
		AddObjects(zCount);
	}
	size_t centerRow = zCount / 2;
	size_t centerColumn = zCount / 2;
	
	float centerX = centerColumn * positionDumpX;
	float centerZ = centerRow * positionDumpZ;
	
	cube->transform.position = Vector3(centerX, -5.f, centerZ);
}

void ShadowTestScene::ImGUIRender()
{
	static bool ShowSphereEdit = false;
	static bool ShowPistolEdit = false;
	static bool ShowCharEdit = false;

	ImGui::Begin("D3DRenderer");
	{
		ImGui::EditD3DRenderer();
	}
	ImGui::End();

	ImGui::Begin("Hierarchy");
	{
		ImGui::EditHierarchyView();
	}
	ImGui::End();

	ImGui::Begin("Debug");
	{
		ImGui::Text("FPS : %d, DRAW CALL : %llu", TimeSystem::Time.GetFrameRate(), d3dRenderer.GetDrawCount());
		ImGui::Button("Show Sphere Edit", &ShowSphereEdit);
		ImGui::Button("Show Pistol Edit", &ShowPistolEdit);
		ImGui::Button("Show Char Edit", &ShowCharEdit);
		ImGui::Checkbox("Draw Light Frustum", &d3dRenderer.DebugSetting.DrawLightFrustum);
		ImGui::Checkbox("Draw Object Bounds", &d3dRenderer.DebugSetting.DrawObjectCullingBox);
		ImGui::EditCamera("Main Camera", pCamera, pCameraMoveHelper);
		ImGui::Image((void*)d3dRenderer.GetShadowMapSRV(0), ImVec2(256, 256));
		ImGui::Image((void*)d3dRenderer.GetShadowMapSRV(1), ImVec2(256, 256));
		ImGui::Image((void*)d3dRenderer.GetShadowMapSRV(2), ImVec2(256, 256));
		ImGui::Image((void*)d3dRenderer.GetShadowMapSRV(3), ImVec2(256, 256));
	}
	ImGui::End();

	ImGui::Begin("Light");
	{
		ImGui::EditLight(DirectionalLight::DirectionalLights.get());
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
				ImGui::EditMaterial(key.c_str(), obj->Material.get());
			}
		}
		ImGui::End();
	}

}

void ShadowTestScene::AddObjects(size_t positionZcount)
{
	auto pistol = NewGameObject(L"pistol");
	auto initMesh = [this](MeshRender* mesh)
		{		
			PBRMeshObject& obj = static_cast<PBRMeshObject&>(mesh->gameObject);
			std::string key = obj.GetNameToString();
			if (pistolMaterials.find(key) == pistolMaterials.end())
				pistolMaterials[key] = obj.Material.get();
			else
			{
				cb_PBRMaterial& material = *(pistolMaterials[key]);
			}
			mesh->textures.SetDefaultTexture(E_TEXTURE::Specular, E_TEXTURE_DEFAULT::ONE);
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
			sphereMaterial = sphere->Material.get();

		sphereMaterial->Albedo = { 1.f, 0.8453f, 0.f, 1.f };
		sphereMaterial->Metalness = 1.f;
		sphereMaterial->Roughness = 0.f;
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
	auto Kachujin = NewGameObject(L"Kachujin");
	Utility::LoadFBX(L"Resource/Kachujin/Hip Hop Dancing.fbx", *Kachujin, false, SURFACE_TYPE::BlingPhong);
	Kachujin->transform.position = Vector3(positionCountX * positionDumpX, 0.f, positionCountZ * positionDumpZ);
	Kachujin->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };
	Kachujin->transform.rotation = Vector3::Up * 23.f;
	Kachujin->GetComponent<TransformAnimation>().PlayClip(L"mixamo.com");

	positionCountX++;
	if (positionZcount == positionCountX)
	{
		positionCountZ++;
		positionCountX = 0;
	}	
}
