#include "PBRTestScene.h"
#include <Framework\SceneManager.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <Framework/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <Framework\ImguiHelper.h>
#include <Utility\AssimpUtility.h>
#include <Framework/TimeSystem.h>
#include <Component/Render/MeshRender.h>
#include <Framework/ResourceManager.h>
#include <Utility/utfConvert.h>
#include <GameObject/Mesh/SphereObject.h>

struct cb_bool
{
	bool useMetalness = true;
	bool pad1[3]{};
	bool useRoughness = true;
	bool pad2[11]{};
};
cb_bool testBool;

#pragma warning(disable : 4305)
PBRTestScene::PBRTestScene()
{
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	d3dRenderer.SetRRState(rasterDesc);

	UseImGUI = true;
	d3dRenderer.backgroundColor = Color(0.3f, 0.3f, 0.3f, 1);

	directionalLight.LightDir = { 0.f,0.f,1.f,0.f };

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0.f, 8.f, -20.f);
	cam->transform.rotation = Vector3(0.f, 0.f, 0.f);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	shared_material = GetResourceManager<cb_PBRMaterial>().GetResource(L"PBR");
	auto initCerberusShader = [this](MeshRender* mesh)
		{ 
			int index = mesh->constBuffer.CreatePSConstantBuffers<cb_PBRDirectionalLight>();
			mesh->constBuffer.BindUpdateEvent(directionalLight);

			index = mesh->constBuffer.CreatePSConstantBuffers<cb_PBRMaterial>();
			mesh->constBuffer.BindUpdateEvent(*shared_material);

			index = mesh->constBuffer.CreatePSConstantBuffers<cb_bool>();
			mesh->constBuffer.BindUpdateEvent(testBool);

			mesh->SetVertexShader(L"Shader/PBRVertexShader.hlsl");
			mesh->SetPixelShader(L"Shader/PBRPixelShader.hlsl");
		};
	auto cerberus = NewGameObject(L"cerberus");
	Utility::LoadFBX(L"Resource/cerberus/cerberus.fbx", *cerberus, initCerberusShader, false);
	cerberus->transform.position += Vector3::Left * 10.f;
	cerberus->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };
	cerberus->transform.rotation = Vector3{ 0.f, 90.f, 0.f };

	auto initCharShader = [this](MeshRender* mesh)
		{
			int index = mesh->constBuffer.CreatePSConstantBuffers<cb_PBRDirectionalLight>();
			mesh->constBuffer.BindUpdateEvent(directionalLight);

			std::string key = utfConvert::wstring_to_utf8(mesh->gameObject.Name);
			auto iter = charMaterialList.find(key);
			if(iter == charMaterialList.end())
				charMaterialList[key] = cb_PBRMaterial(mesh->baseColor);

			index = mesh->constBuffer.CreatePSConstantBuffers<cb_PBRMaterial>();
			mesh->constBuffer.BindUpdateEvent(charMaterialList[key]);

			index = mesh->constBuffer.CreatePSConstantBuffers<cb_bool>();
			mesh->constBuffer.BindUpdateEvent(testBool);

			mesh->SetVertexShader(L"Shader/PBRVertexShader.hlsl");
			mesh->SetPixelShader(L"Shader/PBRPixelShader.hlsl");

			charObjList[key] = &mesh->gameObject;
		};
	auto charater = NewGameObject(L"charater");
	Utility::LoadFBX(L"Resource/char/char.fbx", *charater, initCharShader, false);
	charater->transform.position += Vector3::Right * 10.f;
	charater->transform.rotation = Vector3::Up * 23.f;
	charater->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };

	auto Sphere = NewGameObject<SphereObject>(L"Sphere");
	Sphere->transform.position += Vector3::Up * 15.f;
	{
		SphereMeshRender* mesh = &Sphere->sphereMeshRender;
		mesh->CreateSphere(5.0f, 100, 100);

		mesh->constBuffer.CreatePSConstantBuffers<cb_PBRDirectionalLight>();
		mesh->constBuffer.BindUpdateEvent(directionalLight);

		mesh->constBuffer.CreatePSConstantBuffers<cb_PBRMaterial>();
		sphereMaterial.baseColor = { 1.f, 1.f, 0.f, 1.f };
		sphereMaterial.Metalness = 1.f;
		sphereMaterial.Roughness = 0.f;
		mesh->constBuffer.BindUpdateEvent(sphereMaterial);

		static cb_bool SphereBool;
		SphereBool.useMetalness = false;
		SphereBool.useRoughness = false;
		mesh->constBuffer.CreatePSConstantBuffers<cb_bool>();
		mesh->constBuffer.BindUpdateEvent(SphereBool);

		mesh->SetVertexShader(L"Shader/PBRVertexShader.hlsl");
		mesh->SetPixelShader(L"Shader/PBRPixelShader.hlsl");
	}
}

PBRTestScene::~PBRTestScene()
{
}

void PBRTestScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	static bool showCharEditBox = false;
	static bool showSphereEditBox = false;

	ImGui::Begin("Debug");
	{
		ImGui::Text("FPS : %d", TimeSystem::Time.GetFrameRate());

		if (mainCam)
		{
			ImGui::Text("Camera");
			ImGui::SliderFloat("FOV", &mainCam->FOV, 10, 120);
			ImGui::SliderFloat("CamNear", &mainCam->Near, 0.05f, 10.f);
			ImGui::SliderFloat("CamFar", &mainCam->Far, 100.f, 10000.f);
			ImGui::DragFloat("CamSpeed", pCamSpeed, 1.f ,1.f , 1000.f);
			ImGui::DragVector3("Cam Position", &mainCam->transform.position, 0);
			ImGui::DragQuaternion("Cam Rotation", &mainCam->transform.rotation, 0);
			ImGui::Text("");
		}
		if (ImGui::Button("Charater Material Edit"))
			showCharEditBox = !showCharEditBox;

		if (ImGui::Button("Sphere Material Edit"))
			showSphereEditBox = !showSphereEditBox;

		ImGui::Checkbox("Use Metalness Map", &testBool.useMetalness);
		ImGui::Checkbox("Use Roughness Map", &testBool.useRoughness);

		ImGui::Text("Light");
		ImGui::SliderFloat3("Light Dir", reinterpret_cast<float*>(&directionalLight.LightDir), -1.f, 1.f);
		ImGui::ColorEdit3("Light Diffuse", &directionalLight.LightColor);
		ImGui::ColorEdit3("Light Ambient", &directionalLight.LightAmbient);
		ImGui::DragFloat("Light Intensity", &directionalLight.LightIntensity, 1.0f, 0.f, 100.f);

		ImGui::Text("Background");
		ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	}
	ImGui::End();

	if (showCharEditBox)
	{
		ImGui::Begin("Character Material Editor", &showCharEditBox);
		int id = 0;
		for (auto& [name, material] : charMaterialList)
		{
			ImGui::PushID(id);
			ImGui::Text(name.c_str());
			ImGui::Checkbox("Active", &charObjList[name]->Active);
			ImGui::ColorEdit4("BaseColor", &material.baseColor);
			ImGui::SliderFloat("Metalness", &material.Metalness, 0.f, 1.f);
			ImGui::SliderFloat("Roughness", &material.Roughness, 0.f, 1.f);
			ImGui::Text("\n");
			ImGui::PopID();
			id++;
		}
		ImGui::End();
	}
	if (showSphereEditBox)
	{
		ImGui::Begin("Sphere Material Editor", &showSphereEditBox);
		ImGui::Text("Sphere");
		ImGui::ColorEdit4("BaseColor", &sphereMaterial.baseColor);
		ImGui::SliderFloat("Metalness", &sphereMaterial.Metalness, 0.f, 1.f);
		ImGui::SliderFloat("Roughness", &sphereMaterial.Roughness, 0.f, 1.f);
		ImGui::End();
	}
}
