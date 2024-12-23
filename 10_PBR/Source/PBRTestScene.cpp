#include "PBRTestScene.h"
#include <Manager/SceneManager.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <D3DCore/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <Utility/ImguiHelper.h>
#include <Utility\AssimpUtility.h>
#include <Core/TimeSystem.h>
#include <Component/Render/MeshRender.h>
#include <Manager/ResourceManager.h>
#include <Utility/utfConvert.h>
#include <GameObject/Mesh/SphereObject.h>

struct alignas(16) cb_bool
{
	CBbool useMetalness = true;
	CBbool useRoughness = true;
};
cb_bool testBool;

#pragma warning(disable : 4305)
void PBRTestScene::Start()
{
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	d3dRenderer.SetRRState(rasterDesc);

	UseImGUI = true;
	GuizmoSetting.UseImGuizmo = true;

	d3dRenderer.backgroundColor = Color(0.3f, 0.3f, 0.3f, 1);

	DirectionalLight::DirectionalLights.Lights[0].LightDir = {0.f,0.f,1.f,0.f};

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0.f, 8.f, -20.f);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	auto initCerberusShader = [this](MeshRender* mesh)
		{ 
			std::string key = mesh->gameObject.GetNameToString();
			int index = mesh->constBuffer.CreatePSConstantBuffers<cb_bool>();
			mesh->constBuffer.BindUpdateEvent(testBool);

			mesh->SetVertexShader(L"Shader/PBRVertexShader.hlsl");
			mesh->SetPixelShader(L"Shader/PBRPixelShader.hlsl");

			cerberusObjList[key] = static_cast<PBRMeshObject*>(&mesh->gameObject);
		};
	auto cerberus = NewGameObject(L"cerberus");
	Utility::LoadFBX(L"Resource/cerberus/cerberus.fbx", *cerberus, initCerberusShader, false, SURFACE_TYPE::PBR);
	cerberus->transform.position += Vector3::Left * 10.f;
	cerberus->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };
	cerberus->transform.rotation = Vector3{ 0.f, 90.f, 0.f };

	auto initCharShader = [this](MeshRender* mesh)
		{
			std::string key = mesh->gameObject.GetNameToString();	
			int index = mesh->constBuffer.CreatePSConstantBuffers<cb_bool>();
			mesh->constBuffer.BindUpdateEvent(testBool);

			mesh->SetVertexShader(L"Shader/PBRVertexShader.hlsl");
			mesh->SetPixelShader(L"Shader/PBRPixelShader.hlsl");

			charObjList[key] = static_cast<PBRMeshObject*>(&mesh->gameObject);
		};
	auto charater = NewGameObject(L"charater");
	Utility::LoadFBX(L"Resource/char/char.fbx", *charater, initCharShader, false, SURFACE_TYPE::PBR);
	charater->transform.position += Vector3::Right * 10.f;
	charater->transform.rotation = Vector3::Up * 23.f;
	charater->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };

	SphereObject* Sphere = (SphereObject*)gameObjectFactory.NewGameObjectToKey(typeid(SphereObject).name())(L"Sphere");
	Sphere->transform.position += Vector3::Up * 15.f;
	{
		sphereMaterial = &Sphere->Material;
		sphereMaterial->Albedo = { 1.f, 1.f, 0.f, 1.f };
		sphereMaterial->Metalness = 1.f;
		sphereMaterial->Roughness = 0.f;
	}
}

PBRTestScene::~PBRTestScene()
{
}

void PBRTestScene::ImGUIRender()
{
	using namespace DirectionalLight;

	Camera* mainCam = Camera::GetMainCamera();
	static bool showCerberusEditBox = false;
	static bool showCharEditBox = false;
	static bool showSphereEditBox = false;

	ImGui::Begin("Debug");
	{
		ImGui::Text("FPS : %d", TimeSystem::Time.GetFrameRate());

		if (mainCam)
		{
			ImGui::EditCamera("Main Camera", mainCam, mainCam->IsComponent<CameraMoveHelper>());
		}
		if (ImGui::Button("Cerberus Material Edit"))
			showCerberusEditBox = !showCerberusEditBox;

		if (ImGui::Button("Charater Material Edit"))
			showCharEditBox = !showCharEditBox;

		if (ImGui::Button("Sphere Material Edit"))
			showSphereEditBox = !showSphereEditBox;

		ImGui::Text("");
		ImGui::Checkbox("Use Metalness Map", &testBool.useMetalness);
		ImGui::Checkbox("Use Roughness Map", &testBool.useRoughness);
		ImGui::Text("");

		ImGui::Text("Background");
		ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
		ImGui::Text("");
	}
	ImGui::End();

	ImGui::Begin("Lights");
	{
		ImGui::EditLight(&DirectionalLight::DirectionalLights);
	}
	ImGui::End();

	if (showCerberusEditBox)
	{
		ImGui::Begin("Cerberus Material Editor", &showCerberusEditBox);
		ImGui::Text("Cerberus");
		int id = 0;
		for (auto& [name, obj] : cerberusObjList)
		{
			ImGui::PushID(id);
			ImGui::Text(name.c_str());
			ImGui::Checkbox("Active", &obj->Active);
			ImGui::ColorEdit4("BaseColor", &obj->Material.Albedo);
			ImGui::SliderFloat("Metalness", &obj->Material.Metalness, 0.f, 1.f);
			ImGui::SliderFloat("Roughness", &obj->Material.Roughness, 0.f, 1.f);
			ImGui::Text("");
			ImGui::PopID();
			id++;
		}
		ImGui::End();
	}
	if (showCharEditBox)
	{
		ImGui::Begin("Character Material Editor", &showCharEditBox);
		int id = 0;
		for (auto& [name, obj] : charObjList)
		{
			ImGui::PushID(id);
			ImGui::Text(name.c_str());
			ImGui::Checkbox("Active", &obj->Active);
			ImGui::ColorEdit4("BaseColor", &obj->Material.Albedo);
			ImGui::SliderFloat("Metalness", &obj->Material.Metalness, 0.f, 1.f);
			ImGui::SliderFloat("Roughness", &obj->Material.Roughness, 0.f, 1.f);
			ImGui::Text("");
			ImGui::PopID();
			id++;
		}
		ImGui::End();
	}
	if (showSphereEditBox)
	{
		ImGui::Begin("Sphere Material Editor", &showSphereEditBox);
		ImGui::Text("Sphere");
		ImGui::ColorEdit4("BaseColor", &sphereMaterial->Albedo);
		ImGui::SliderFloat("Metalness", &sphereMaterial->Metalness, 0.f, 1.f);
		ImGui::SliderFloat("Roughness", &sphereMaterial->Roughness, 0.f, 1.f);
		ImGui::End();
	}
}
