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
				curr->AddComponent<SimpleUpdateCbuffer>().Material = meshRender->Material;
				SimpleMaterial* Material = curr->GetComponent<SimpleUpdateCbuffer>().Material;
				Material->SetVS(L"VertexShader.hlsl");
				Material->SetPS(L"PixelShader.hlsl");
				Material->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
				Material->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
				Material->cbuffer.CreatePSConstantBuffers<cb_localBool>();

				Material->cbuffer.BindUpdateEvent(curr->GetComponent<SimpleUpdateCbuffer>().cb_localbool);
				Material->cbuffer.BindUpdateEvent(Global_Cbuffer::cb_bool);
			}
		}

		for (unsigned int i = 0; i < curr->transform.GetChildCount(); i++)
		{
			que.push(&curr->transform.GetChild(i)->gameObject);
		}
	}
}

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
	Utility::LoadFBX("Resource/Character.fbx", *character);
	AddUpdateCbufferAllChild(character);
	
	auto zelda = NewGameObject<GameObject>(L"zelda");
	zelda->transform.position = { 0,0,0 };
	zelda->transform.scale = { 0.1,0.1,0.1 };
	Utility::LoadFBX("Resource/zeldaPosed001.fbx", *zelda);
	AddUpdateCbufferAllChild(zelda);

	auto tree = NewGameObject<GameObject>(L"Tree");
	tree->transform.position = { -15,0,0 };
	tree->transform.scale = { 10, 10, 10 };
	Utility::LoadFBX("Resource/Tree.fbx", *tree, true);
	AddUpdateCbufferAllChild(tree);

	auto box = NewGameObject<GameObject>(L"box");
	box->transform.position = { -5, 5, -15 };
	box->transform.scale = { 0.1, 0.1, 0.1 };
	Utility::LoadFBX("Resource/box.fbx", *box, true);
	AddUpdateCbufferAllChild(box);

	auto monkey = NewGameObject<GameObject>(L"Monkey");
	monkey->transform.position = { 0,5,30 };
	monkey->transform.scale = { 0.05,0.05,0.05 };
	Utility::LoadFBX("Resource/Monkey.fbx", *monkey);
	AddUpdateCbufferAllChild(monkey);

	auto torus = NewGameObject<GameObject>(L"Torus");
	torus->transform.position = { 30,5,30 };
	torus->transform.scale = { 0.05,0.05,0.05 };
	Utility::LoadFBX("Resource/Torus.fbx", *torus);
	AddUpdateCbufferAllChild(torus);

	auto IcoSphere = NewGameObject<GameObject>(L"IcoSphere");
	IcoSphere->transform.position = { -30, 5, 30 };
	IcoSphere->transform.scale = { 0.05,0.05,0.05 };
	Utility::LoadFBX("Resource/IcoSphere.fbx", *IcoSphere);
	AddUpdateCbufferAllChild(IcoSphere);
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