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
	character->AddComponent<SimpleUpdateCbuffer>().myKey = L"Character";
	Utility::LoadFBX("Resource/Character.fbx", *character, L"Character");
	materialManager[L"Character"]->SetVS(L"VertexShader.hlsl");
	materialManager[L"Character"]->SetPS(L"PixelShader.hlsl");
	materialManager[L"Character"]->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
	materialManager[L"Character"]->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
	materialManager[L"Character"]->cbuffer.CreatePSConstantBuffers<cb_localBool>();

	auto zelda = NewGameObject<GameObject>(L"zelda");
	zelda->transform.position = { 0,0,0 };
	zelda->transform.scale = { 0.1,0.1,0.1 };
	zelda->AddComponent<SimpleUpdateCbuffer>().myKey = L"zelda";
	Utility::LoadFBX("Resource/zeldaPosed001.fbx", *zelda, L"zelda");
	materialManager[L"zelda"]->SetVS(L"VertexShader.hlsl");
	materialManager[L"zelda"]->SetPS(L"PixelShader.hlsl");
	materialManager[L"zelda"]->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
	materialManager[L"zelda"]->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
	materialManager[L"zelda"]->cbuffer.CreatePSConstantBuffers<cb_localBool>();

	auto tree = NewGameObject<GameObject>(L"Tree");
	tree->transform.position = { -15,0,0 };
	tree->transform.scale = { 10, 10, 10 };
	tree->AddComponent<SimpleUpdateCbuffer>().myKey = L"Tree";
	Utility::LoadFBX("Resource/Tree.fbx", *tree, L"Tree");
	materialManager[L"Tree"]->SetVS(L"VertexShader.hlsl");
	materialManager[L"Tree"]->SetPS(L"PixelShader.hlsl");
	materialManager[L"Tree"]->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
	materialManager[L"Tree"]->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
	materialManager[L"Tree"]->cbuffer.CreatePSConstantBuffers<cb_localBool>();

	auto box = NewGameObject<GameObject>(L"box");
	box->transform.position = { -5, 5, -15 };
	box->transform.scale = { 0.1, 0.1, 0.1 };
	box->AddComponent<SimpleUpdateCbuffer>().myKey = L"box";
	Utility::LoadFBX("Resource/box.fbx", *box, L"box");
	materialManager[L"box"]->SetVS(L"VertexShader.hlsl");
	materialManager[L"box"]->SetPS(L"PixelShader.hlsl");
	materialManager[L"box"]->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
	materialManager[L"box"]->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
	materialManager[L"box"]->cbuffer.CreatePSConstantBuffers<cb_localBool>();

	auto monkey = NewGameObject<GameObject>(L"Monkey");
	monkey->transform.position = { 0,5,30 };
	monkey->transform.scale = { 0.05,0.05,0.05 };
	monkey->AddComponent<SimpleUpdateCbuffer>().myKey = L"Monkey";
	Utility::LoadFBX("Resource/Monkey.fbx", *monkey, L"Monkey");
	materialManager[L"Monkey"]->SetVS(L"VertexShader.hlsl");
	materialManager[L"Monkey"]->SetPS(L"PixelShader.hlsl");
	materialManager[L"Monkey"]->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
	materialManager[L"Monkey"]->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
	materialManager[L"Monkey"]->cbuffer.CreatePSConstantBuffers<cb_localBool>();

	auto torus = NewGameObject<GameObject>(L"Torus");
	torus->transform.position = { 30,5,30 };
	torus->transform.scale = { 0.05,0.05,0.05 };
	torus->AddComponent<SimpleUpdateCbuffer>().myKey = L"Torus";
	Utility::LoadFBX("Resource/Torus.fbx", *torus, L"Torus");
	materialManager[L"Torus"]->SetVS(L"VertexShader.hlsl");
	materialManager[L"Torus"]->SetPS(L"PixelShader.hlsl");
	materialManager[L"Torus"]->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
	materialManager[L"Torus"]->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
	materialManager[L"Torus"]->cbuffer.CreatePSConstantBuffers<cb_localBool>();

	auto IcoSphere = NewGameObject<GameObject>(L"IcoSphere");
	IcoSphere->transform.position = { -30, 5, 30 };
	IcoSphere->transform.scale = { 0.05,0.05,0.05 };
	IcoSphere->AddComponent<SimpleUpdateCbuffer>().myKey = L"IcoSphere";
	Utility::LoadFBX("Resource/IcoSphere.fbx", *IcoSphere, L"IcoSphere");
	materialManager[ L"IcoSphere"]->SetVS(L"VertexShader.hlsl");
	materialManager[ L"IcoSphere"]->SetPS(L"PixelShader.hlsl");
	materialManager[L"IcoSphere"]->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
	materialManager[ L"IcoSphere"]->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
	materialManager[ L"IcoSphere"]->cbuffer.CreatePSConstantBuffers<cb_localBool>();
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
	ImGui::DragFloat3("LightDir", (float *)&cb_Light.LightDir, 0.01f, -1.0f, 1.0f);
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