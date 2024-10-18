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

#pragma warning(disable : 4305)
TransAnimationScene::TransAnimationScene()
{
	SimpleDirectionalLight::cb_Light.LightDir = { 0.5, 0, 1, 0 };

	UseImGUI = true;
	d3dRenderer.backgroundColor = Color(0, 0, 0, 1);

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0, 30, -50);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	box = NewGameObject<GameObject>(L"BoxHuman");
	box->transform.position = { 0,0,0 };
	box->transform.scale = { 0.05,0.05,0.05 };

	materialManager.GetMaterial(L"BoxHuman")->SetVS(L"VertexShader.hlsl");
	materialManager.GetMaterial(L"BoxHuman")->SetPS(L"PixelShader.hlsl");
	materialManager.GetMaterial(L"BoxHuman")->cb_material.MaterialDiffuse = { 0.76f ,0.76f ,0.76f ,1.f };
	materialManager[L"BoxHuman"]->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
	Utility::LoadFBX("Resource/Robot_Dummy_class.fbx", *box, materialManager[L"BoxHuman"]);

	box->GetComponent<TransformAnimation>().PlayClip(L"Scene", true);	
}

TransAnimationScene::~TransAnimationScene()
{
}

void TransAnimationScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	cbuffer_Light& cb_Light = SimpleDirectionalLight::cb_Light;
	cb_Material& cb_material = materialManager[L"BoxHuman"]->cb_material;

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

	ImGui::ColorEdit3("MaterialDiffuse", &cb_material.MaterialDiffuse);
	ImGui::ColorEdit3("MaterialAmbient", &cb_material.MaterialAmbient);
	ImGui::ColorEdit3("MaterialSpecular", &cb_material.MaterialSpecular);
	ImGui::DragFloat("MaterialSpecularPower", &cb_material.MaterialSpecularPower, 10, 50, 500);
	ImGui::Text("");

	ImGui::Text("Box Robot");
	ImGui::DragQuaternion("Box rotation", &box->transform.rotation);
	ImGui::Text("");

	ImGui::Text("Background");
	ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	ImGui::End();
}
#pragma warning(default : 4305)