#include "ImportFBXScene.h"
#include <Manager/SceneManager.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <D3DCore/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <Utility/ImguiHelper.h>
#include <Math\Mathf.h>
#include <Light\SimpleDirectionalLight.h>
#include <Utility\AssimpUtility.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Component/Render/SimpleBoneMeshRender.h>
#include <Component/Animation/TransformAnimation.h>
#include <Manager/ResourceManager.h>

#include "../Source/SimpleUpdateCbuffer.h"
#include "../Source/Global_Cbuffer.h"

#pragma warning(disable : 4305)
void AddUpdateCbufferAllChild(GameObject* root)
{
	std::queue<GameObject*> que;
	que.push(root);

	GameObject* curr = nullptr;
	while (!que.empty())
	{
		curr = que.front();
 		que.pop();

		for (int i = 0; i < curr->GetComponentCount(); i++)
		{
			if (SimpleMeshRender* meshRender = curr->GetComponentAtIndex<SimpleMeshRender>(i))
			{
				curr->AddComponent<SimpleUpdateCbuffer>().meshRender = meshRender;
			}
			else if(SimpleBoneMeshRender* meshRender = curr->GetComponentAtIndex<SimpleBoneMeshRender>(i))
			{
				curr->AddComponent<SimpleUpdateCbuffer>().meshRender = meshRender;
			}
		}

		for (unsigned int i = 0; i < curr->transform.GetChildCount(); i++)
		{
			que.push(&curr->transform.GetChild(i)->gameObject);
		}
	}
}

void ImportFBXScene::Start()
{
	UseImGUI = true;
	d3dRenderer.backgroundColor = Color(0.5, 0.5, 0.5, 1);

	auto initMesh = [](MeshRender* mesh)
		{
			mesh->RenderFlags |= RENDER_FORWARD;
			int index = mesh->constBuffer.CreatePSConstantBuffers<cb_BlingPhongMaterial>(Global_Cbuffer::cb_material_key);
			index = mesh->constBuffer.CreatePSConstantBuffers<cb_DirectionalLight>(SimpleDirectionalLight::cb_light_key);
			mesh->constBuffer.CreatePSConstantBuffers<cbuffer_bool>(Global_Cbuffer::cb_bool_key);
			mesh->constBuffer.CreatePSConstantBuffers<cb_localBool>(mesh->gameObject.GetNameToString().c_str());

			mesh->SetVertexShader(L"VertexShader.hlsl");
			mesh->SetPixelShader(L"PixelShader.hlsl");
		};

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0, 16, -30);
	cam->transform.rotation = Quaternion::CreateFromYawPitchRoll(Vector3(-3, 0, 0) * Mathf::Deg2Rad);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	auto character = Utility::LoadFBX(L"Resource/Character.fbx", initMesh, false, SURFACE_TYPE::NONE);
	character->transform.position = { 15,0,0 };
	character->transform.scale = { 0.1,0.1,0.1 };
	AddUpdateCbufferAllChild(character);										 
	
	auto zelda = Utility::LoadFBX(L"Resource/zeldaPosed001.fbx", initMesh, false, SURFACE_TYPE::NONE);
	zelda->transform.position = { 0,0,0 };
	zelda->transform.scale = { 0.1,0.1,0.1 };
	AddUpdateCbufferAllChild(zelda);

	auto tree = Utility::LoadFBX(L"Resource/Tree.fbx", initMesh, true, SURFACE_TYPE::NONE);
	tree->transform.position = { -15,0,0 };
	tree->transform.scale = { 10, 10, 10 };
	AddUpdateCbufferAllChild(tree);

	auto box = Utility::LoadFBX(L"Resource/box.fbx", initMesh, true, SURFACE_TYPE::NONE);
	box->transform.position = { -5, 5, -15 };
	box->transform.scale = { 0.1, 0.1, 0.1 };
	AddUpdateCbufferAllChild(box);

	auto monkey = Utility::LoadFBX(L"Resource/Monkey.fbx", initMesh, false, SURFACE_TYPE::NONE);
	monkey->transform.position = { 0,5,30 };
	monkey->transform.scale = { 0.05,0.05,0.05 };
	AddUpdateCbufferAllChild(monkey);

	auto torus = Utility::LoadFBX(L"Resource/Torus.fbx", initMesh, false, SURFACE_TYPE::NONE);
	torus->transform.position = { 30,5,30 };
	torus->transform.scale = { 0.05,0.05,0.05 };
	AddUpdateCbufferAllChild(torus);

	auto IcoSphere = Utility::LoadFBX(L"Resource/IcoSphere.fbx", initMesh, false, SURFACE_TYPE::NONE);
	IcoSphere->transform.position = { -30, 5, 30 };
	IcoSphere->transform.scale = { 0.05,0.05,0.05 };
	AddUpdateCbufferAllChild(IcoSphere);
}

ImportFBXScene::~ImportFBXScene()
{
}

void ImportFBXScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	cb_DirectionalLight& cb_light = *SimpleDirectionalLight::cb_light;
	cbuffer_bool& cb_bool = *Global_Cbuffer::cb_bool;
	cb_BlingPhongMaterial& cb_material = *Global_Cbuffer::cb_material;

	ImGui::Begin("Debug");
	ImGui::Text("Camera");
	ImGui::SliderFloat("FOV", &mainCam->FOV, 10, 120);
	ImGui::SliderFloat("CamSpeed", pCamSpeed, 1, 1000);
	ImGui::DragVector3("Cam Position", &mainCam->transform.position, 0);
	ImGui::DragQuaternionWorld("Cam Rotation", &mainCam->transform.rotation, 0);
	ImGui::Text("");

	ImGui::Text("Light");
	ImGui::DragFloat3("LightDir", (float*)&cb_light.LightDir, 0.01f, -1.0f, 1.0f);
	ImGui::ColorEdit3("LightDiffuse", &cb_light.LightColor);
	ImGui::ColorEdit3("LightAmbient", &cb_light.LightAmbient);
	ImGui::ColorEdit3("LightSpecular", &cb_light.LightSpecular);
	ImGui::Text("");

	ImGui::Text("SimpleMaterial");
	ImGui::Checkbox("Use NormalMap", &cb_bool.UseNormalMap);
	ImGui::Checkbox("Use SpecularMap", &cb_bool.UseSpecularMap);
	ImGui::Checkbox("Use EmissiveMap", &cb_bool.UseEmissiveMap);
	ImGui::Checkbox("Use OpacityMap", &cb_bool.UseOpacityMap);

	ImGui::ColorEdit3("MaterialDiffuse", &cb_material.MaterialDiffuse);
	ImGui::ColorEdit3("MaterialAmbient", &cb_material.MaterialAmbient);
	ImGui::ColorEdit3("MaterialSpecular", &cb_material.MaterialSpecular);
	ImGui::DragFloat("MaterialSpecularPower", &cb_material.MaterialSpecularPower, 10, 50, 500);
	ImGui::Text("");

	ImGui::Text("Background");
	ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	ImGui::End();
}
#pragma warning(default : 4305)