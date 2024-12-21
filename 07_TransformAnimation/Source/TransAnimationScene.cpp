#include "TransAnimationScene.h"
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

#pragma warning(disable : 4305)
void TransAnimationScene::Start()
{
	SimpleDirectionalLight::cb_light.LightDir = { 0.5, 0, 1, 0 };

	UseImGUI = true;
	d3dRenderer.backgroundColor = Color(0, 0, 0, 1);

	material_01 = GetResourceManager<cb_BlingPhongMaterial>().GetResource(L"BlingPhong");

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0, 30, -50);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	box = NewGameObject<GameObject>(L"BoxHuman");
	box->transform.position = { 0,0,0 };
	box->transform.scale = { 0.05,0.05,0.05 };

	auto meshInit = [this](MeshRender* mesh) 
		{
			int index = mesh->constBuffer.CreatePSConstantBuffers<cb_BlingPhongMaterial>();
			mesh->constBuffer.BindUpdateEvent(*material_01);

			index = mesh->constBuffer.CreatePSConstantBuffers<cb_DirectionalLight>();
			mesh->constBuffer.BindUpdateEvent(SimpleDirectionalLight::cb_light);

			mesh->SetVertexShader(L"VertexShader.hlsl");
			mesh->SetPixelShader(L"PixelShader.hlsl");
		};
	Utility::LoadFBX(L"Resource/Robot_Dummy_class.fbx", *box, meshInit, false, SURFACE_TYPE::NONE);
	material_01->MaterialDiffuse = { 0.76f ,0.76f ,0.76f ,1.f };

	box->GetComponent<TransformAnimation>().PlayClip(L"Scene", true);
}

TransAnimationScene::~TransAnimationScene()
{
}

void TransAnimationScene::ImGUIRender()
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

	ImGui::ColorEdit3("MaterialDiffuse", &material_01->MaterialDiffuse);
	ImGui::ColorEdit3("MaterialAmbient", &material_01->MaterialAmbient);
	ImGui::ColorEdit3("MaterialSpecular", &material_01->MaterialSpecular);
	ImGui::DragFloat("MaterialSpecularPower", &material_01->MaterialSpecularPower, 10, 50, 500);
	ImGui::Text("");

	ImGui::Text("Box Robot");
	ImGui::DragQuaternionWorld("Box rotation", &box->transform.rotation);
	ImGui::Text("");

	ImGui::Text("Background");
	ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	ImGui::End();
}
#pragma warning(default : 4305)