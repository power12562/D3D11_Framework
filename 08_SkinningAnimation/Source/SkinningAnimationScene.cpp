#include "SkinningAnimationScene.h"
#include <Manager/SceneManager.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <D3DCore/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <Utility/ImguiHelper.h>
#include <Math\Mathf.h>
#include <Component/Animation/TransformAnimation.h>
#include <Light\SimpleDirectionalLight.h>
#include <Utility\AssimpUtility.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Manager/ResourceManager.h>
#include <Material/BlingPhongMaterial.h>

#pragma warning(disable : 4305)
void SkinningAnimationScene::Start()
{
	SimpleDirectionalLight::cb_light.LightDir = { 0.5, 0, 1, 0 };

	UseImGUI = true;
	d3dRenderer.backgroundColor = Color(0, 0, 0, 1);

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0, 10, -25);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	test1 = NewGameObject<GameObject>(L"test1");
	test1->transform.position = { 20,0,0 };
	test1->transform.scale = { 0.1,0.1,0.1 };

	test2 = NewGameObject<GameObject>(L"test2");
	test2->transform.position = {-20,0,0 };
	test2->transform.scale = { 0.1,0.1,0.1 };

	test3 = NewGameObject<GameObject>(L"test3");
	test3->transform.position = { 0,0,0 };
	test3->transform.scale = { 0.1,0.1,0.1 };

	auto testInit = [this](MeshRender* mesh)->void
		{
			MaterialList.emplace_back();
			cb_BlingPhongMaterial& material = MaterialList.back();
			int index = mesh->constBuffer.CreatePSConstantBuffers<cb_BlingPhongMaterial>();
			mesh->constBuffer.BindUpdateEvent(material);
			material.MaterialDiffuse = mesh->baseColor;
				
			index = mesh->constBuffer.CreatePSConstantBuffers<cb_DirectionalLight>();
			mesh->constBuffer.BindUpdateEvent(SimpleDirectionalLight::cb_light);

			mesh->SetVertexShader(L"VertexSkinningShader.hlsl");					
			mesh->SetPixelShader(L"PixelShader.hlsl");	
		};

	Utility::LoadFBX(L"Resource/SkinningTest.fbx", *test1, testInit, false, SURFACE_TYPE::NONE);
	Utility::LoadFBX(L"Resource/SkinningTest2.fbx", *test2, testInit, false, SURFACE_TYPE::NONE);
	Utility::LoadFBX(L"Resource/Hip Hop Dancing.fbx", *test3, testInit, false, SURFACE_TYPE::NONE);

	test1->GetComponent<TransformAnimation>().PlayClip(L"mixamo.com");
	test2->GetComponent<TransformAnimation>().PlayClip(L"mixamo.com");
	test3->GetComponent<TransformAnimation>().PlayClip(L"mixamo.com");
}

SkinningAnimationScene::~SkinningAnimationScene()
{
}

void SkinningAnimationScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	cb_DirectionalLight& cb_light = SimpleDirectionalLight::cb_light;

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

	ImGui::Text("Test1");
	ImGui::DragVector3("Test1 Position", &test1->transform.position);
	ImGui::DragQuaternionWorld("Test1 Rotation", &test1->transform.rotation);
	ImGui::Text("");

	ImGui::Text("Test2");
	ImGui::DragVector3("Test2 Position", &test2->transform.position);
	ImGui::DragQuaternionWorld("Test2 Rotation", &test2->transform.rotation);
	ImGui::Text("");

	ImGui::Text("Background");
	ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	ImGui::End();
}
#pragma warning(default : 4305)