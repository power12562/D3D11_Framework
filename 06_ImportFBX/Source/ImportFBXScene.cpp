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

				meshRender->constBuffer.CreatePSConstantBuffers<cb_DirectionalLight>();
				meshRender->constBuffer.CreatePSConstantBuffers<cbuffer_bool>();
				meshRender->constBuffer.CreatePSConstantBuffers<cb_localBool>();

				meshRender->constBuffer.BindUpdateEvent(curr->GetComponent<SimpleUpdateCbuffer>().cb_localbool);
				meshRender->constBuffer.BindUpdateEvent(Global_Cbuffer::cb_bool);
			}
			else if(SimpleBoneMeshRender* meshRender = curr->GetComponentAtIndex<SimpleBoneMeshRender>(i))
			{
				curr->AddComponent<SimpleUpdateCbuffer>().meshRender = meshRender;

				meshRender->constBuffer.CreatePSConstantBuffers<cb_DirectionalLight>();
				meshRender->constBuffer.CreatePSConstantBuffers<cbuffer_bool>();
				meshRender->constBuffer.CreatePSConstantBuffers<cb_localBool>();

				meshRender->constBuffer.BindUpdateEvent(curr->GetComponent<SimpleUpdateCbuffer>().cb_localbool);
				meshRender->constBuffer.BindUpdateEvent(Global_Cbuffer::cb_bool);
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
			int index = mesh->constBuffer.CreatePSConstantBuffers<cb_BlingPhongMaterial>();
			mesh->constBuffer.BindUpdateEvent(Global_Cbuffer::cb_material);

			index = mesh->constBuffer.CreatePSConstantBuffers<cb_DirectionalLight>();
			mesh->constBuffer.BindUpdateEvent(SimpleDirectionalLight::cb_light);

			mesh->SetVertexShader(L"VertexShader.hlsl");
			mesh->SetPixelShader(L"PixelShader.hlsl");
		};

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0, 16, -30);
	cam->transform.rotation = Quaternion::CreateFromYawPitchRoll(Vector3(-3, 0, 0) * Mathf::Deg2Rad);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	auto character = NewGameObject<GameObject>(L"Character");
	character->transform.position = { 15,0,0 };
	character->transform.scale = { 0.1,0.1,0.1 };
	Utility::LoadFBX(L"Resource/Character.fbx",*character, initMesh, false, SURFACE_TYPE::NONE);
	AddUpdateCbufferAllChild(character);										 
	
	auto zelda = NewGameObject<GameObject>(L"zelda");
	zelda->transform.position = { 0,0,0 };
	zelda->transform.scale = { 0.1,0.1,0.1 };
	Utility::LoadFBX(L"Resource/zeldaPosed001.fbx", *zelda, initMesh, false, SURFACE_TYPE::NONE);
	AddUpdateCbufferAllChild(zelda);

	auto tree = NewGameObject<GameObject>(L"Tree");
	tree->transform.position = { -15,0,0 };
	tree->transform.scale = { 10, 10, 10 };
	Utility::LoadFBX(L"Resource/Tree.fbx", *tree, initMesh, true, SURFACE_TYPE::NONE);
	AddUpdateCbufferAllChild(tree);

	auto box = NewGameObject<GameObject>(L"box");
	box->transform.position = { -5, 5, -15 };
	box->transform.scale = { 0.1, 0.1, 0.1 };
	Utility::LoadFBX(L"Resource/box.fbx", *box, initMesh, true, SURFACE_TYPE::NONE);
	AddUpdateCbufferAllChild(box);

	auto monkey = NewGameObject<GameObject>(L"Monkey");
	monkey->transform.position = { 0,5,30 };
	monkey->transform.scale = { 0.05,0.05,0.05 };
	Utility::LoadFBX(L"Resource/Monkey.fbx", *monkey, initMesh, false, SURFACE_TYPE::NONE);
	AddUpdateCbufferAllChild(monkey);

	auto torus = NewGameObject<GameObject>(L"Torus");
	torus->transform.position = { 30,5,30 };
	torus->transform.scale = { 0.05,0.05,0.05 };
	Utility::LoadFBX(L"Resource/Torus.fbx", *torus, initMesh, false, SURFACE_TYPE::NONE);
	AddUpdateCbufferAllChild(torus);

	auto IcoSphere = NewGameObject<GameObject>(L"IcoSphere");
	IcoSphere->transform.position = { -30, 5, 30 };
	IcoSphere->transform.scale = { 0.05,0.05,0.05 };
	Utility::LoadFBX(L"Resource/IcoSphere.fbx", *IcoSphere, initMesh, false, SURFACE_TYPE::NONE);
	AddUpdateCbufferAllChild(IcoSphere);
}

ImportFBXScene::~ImportFBXScene()
{
}

void ImportFBXScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	cb_DirectionalLight& cb_light = SimpleDirectionalLight::cb_light;
	cbuffer_bool& cb_bool = Global_Cbuffer::cb_bool;
	cb_BlingPhongMaterial& cb_material = Global_Cbuffer::cb_material;

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