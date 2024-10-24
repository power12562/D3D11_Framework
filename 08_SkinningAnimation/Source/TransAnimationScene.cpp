#include "TransAnimationScene.h"
#include <Framework\SceneManager.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <Framework/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <Framework\ImguiHelper.h>
#include <Math\Mathf.h>
#include <Component\TransformAnimation.h>
#include <Light\SimpleDirectionalLight.h>
#include <Utility\AssimpUtility.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Framework\MaterialManager.h>
#include <Material/SimpleMaterial.h>

#pragma warning(disable : 4305)
TransAnimationScene::TransAnimationScene()
{
	SimpleDirectionalLight::cb_Light.LightDir = { 0.5, 0, 1, 0 };

	UseImGUI = true;
	d3dRenderer.backgroundColor = Color(0, 0, 0, 1);

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0, 10, -25);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	test1 = NewGameObject<GameObject>(L"test1");
	test1->transform.position = { 10,0,0 };
	test1->transform.scale = { 0.1,0.1,0.1 };

	test2 = NewGameObject<GameObject>(L"test2");
	test2->transform.position = {-10,0,0 };
	test2->transform.scale = { 0.1,0.1,0.1 };

	auto testInit = [](SimpleMaterial* material)->void
		{
			material->SetVS(L"VertexShader.hlsl");				
			material->SetPS(L"PixelShader.hlsl");	
		};

	Utility::LoadFBX("Resource/SkinningTest.fbx", *test1, nullptr, false, testInit);
	Utility::LoadFBX("Resource/SkinningTest2.fbx", *test2, nullptr, false, testInit);
}

TransAnimationScene::~TransAnimationScene()
{
}

void TransAnimationScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	cbuffer_Light& cb_Light = SimpleDirectionalLight::cb_Light;

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

	ImGui::Text("Test1");
	ImGui::DragVector3("Test1 Position", &test1->transform.position);
	ImGui::Text("");

	ImGui::Text("Test2");
	ImGui::DragVector3("Test2 Position", &test2->transform.position);
	ImGui::Text("");

	ImGui::Text("Background");
	ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	ImGui::End();
}
#pragma warning(default : 4305)