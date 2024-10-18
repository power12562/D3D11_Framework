#include "ImportFBXScene.h"
#include <Framework\SceneManager.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <Framework/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <Framework\ImguiHelper.h>
#include <Math\Mathf.h>
#include <Light\SimpleDirectionalLight.h>
#include <Utility\AssimpUtility.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Framework\MaterialManager.h>

#include "../Source/SimpleUpdateCbuffer.h"
#include "../Source/Global_Cbuffer.h"

#pragma warning(disable : 4305)
ImportFBXScene::ImportFBXScene()
{
	UseImGUI = true;
	d3dRenderer.backgroundColor = Color(0.5, 0.5, 0.5, 1);

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0, 16, -30);
	cam->transform.rotation = Quaternion::CreateFromYawPitchRoll(Vector3(-3, 0, 0) * Mathf::Deg2Rad);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	auto character = NewGameObject<GameObject>(L"Character");
	character->transform.position = { 15,0,0 };
	character->transform.scale = { 0.1,0.1,0.1 };
	auto list = Utility::LoadFBX("Resource/Character.fbx", *character);
	for (auto& i : list)
	{
		i->SetVS(L"VertexShader.hlsl");
		i->SetPS(L"PixelShader.hlsl");
		i->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
		i->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
		i->cbuffer.CreatePSConstantBuffers<cb_localBool>();
	}
	character->AddComponent<SimpleUpdateCbuffer>().list = list;

	auto zelda = NewGameObject<GameObject>(L"zelda");
	zelda->transform.position = { 0,0,0 };
	zelda->transform.scale = { 0.1,0.1,0.1 };
	list = Utility::LoadFBX("Resource/zeldaPosed001.fbx", *zelda);
	for (auto& i : list)
	{
		i->SetVS(L"VertexShader.hlsl");
		i->SetPS(L"PixelShader.hlsl");
		i->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
		i->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
		i->cbuffer.CreatePSConstantBuffers<cb_localBool>();
	}
	zelda->AddComponent<SimpleUpdateCbuffer>().list = list;

	auto tree = NewGameObject<GameObject>(L"Tree");
	tree->transform.position = { -15,0,0 };
	tree->transform.scale = { 10, 10, 10 };
	list = Utility::LoadFBX("Resource/Tree.fbx", *tree);
	for (auto& i : list)
	{
		i->SetVS(L"VertexShader.hlsl");
		i->SetPS(L"PixelShader.hlsl");
		i->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
		i->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
		i->cbuffer.CreatePSConstantBuffers<cb_localBool>();
	}
	tree->AddComponent<SimpleUpdateCbuffer>().list = list;

	auto box = NewGameObject<GameObject>(L"box");
	box->transform.position = { -5, 5, -15 };
	box->transform.scale = { 0.1, 0.1, 0.1 };
	list = Utility::LoadFBX("Resource/box.fbx", *box);
	for (auto& i : list)
	{
		i->SetVS(L"VertexShader.hlsl");
		i->SetPS(L"PixelShader.hlsl");
		i->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
		i->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
		i->cbuffer.CreatePSConstantBuffers<cb_localBool>();
	}
	box->AddComponent<SimpleUpdateCbuffer>().list = list;

	auto monkey = NewGameObject<GameObject>(L"Monkey");
	monkey->transform.position = { 0,5,30 };
	monkey->transform.scale = { 0.05,0.05,0.05 };
	list = Utility::LoadFBX("Resource/Monkey.fbx", *monkey);
	for (auto& i : list)
	{
		i->SetVS(L"VertexShader.hlsl");
		i->SetPS(L"PixelShader.hlsl");
		i->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
		i->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
		i->cbuffer.CreatePSConstantBuffers<cb_localBool>();
	}
	monkey->AddComponent<SimpleUpdateCbuffer>().list = list;

	auto torus = NewGameObject<GameObject>(L"Torus");
	torus->transform.position = { 30,5,30 };
	torus->transform.scale = { 0.05,0.05,0.05 };
	list = Utility::LoadFBX("Resource/Torus.fbx", *torus);
	for (auto& i : list)
	{
		i->SetVS(L"VertexShader.hlsl");
		i->SetPS(L"PixelShader.hlsl");
		i->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
		i->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
		i->cbuffer.CreatePSConstantBuffers<cb_localBool>();
	}
	torus->AddComponent<SimpleUpdateCbuffer>().list = list;

	auto IcoSphere = NewGameObject<GameObject>(L"IcoSphere");
	IcoSphere->transform.position = { -30, 5, 30 };
	IcoSphere->transform.scale = { 0.05,0.05,0.05 };
	list = Utility::LoadFBX("Resource/IcoSphere.fbx", *IcoSphere);
	for (auto& i : list)
	{
		i->SetVS(L"VertexShader.hlsl");
		i->SetPS(L"PixelShader.hlsl");
		i->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
		i->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
		i->cbuffer.CreatePSConstantBuffers<cb_localBool>();
	}
	IcoSphere->AddComponent<SimpleUpdateCbuffer>().list = list;
}

ImportFBXScene::~ImportFBXScene()
{
}

void ImportFBXScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	cbuffer_Light& cb_Light = SimpleDirectionalLight::cb_Light;
	cbuffer_bool& cb_bool = Global_Cbuffer::cb_bool;
	cb_Material& cb_material = Global_Cbuffer::cb_material;

	ImGui::Begin("Debug");
	ImGui::Text("Camera");
	ImGui::SliderFloat("FOV", &mainCam->FOV, 10, 120);
	ImGui::SliderFloat("CamSpeed", pCamSpeed, 1, 1000);
	ImGui::DragVector3("Cam Position", &mainCam->transform.position, 0);
	ImGui::DragQuaternion("Cam Rotation", &mainCam->transform.rotation, 0);
	ImGui::Text("");

	ImGui::Text("Light");
	ImGui::DragFloat3("LightDir", (float*)&cb_Light.LightDir, 0.01f, -1.0f, 1.0f);
	ImGui::ColorEdit3("LightDiffuse", &cb_Light.LightDiffuse);
	ImGui::ColorEdit3("LightAmbient", &cb_Light.LightAmbient);
	ImGui::ColorEdit3("LightSpecular", &cb_Light.LightSpecular);
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