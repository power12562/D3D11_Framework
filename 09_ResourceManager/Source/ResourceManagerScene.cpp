#include "ResourceManagerScene.h"
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
#include <Framework\ResourceManager.h>
#include <Material/SimpleMaterial.h>

#pragma warning(disable : 4305)
ResourceManagerScene::ResourceManagerScene()
{
	SimpleDirectionalLight::cb_Light.LightDir = { 0.5, 0, 1, 0 };

	UseImGUI = true;
	d3dRenderer.backgroundColor = Color(0, 0, 0, 1);

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0, 30, -25);
	cam->transform.rotation = Vector3(-15, 0, 0);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	test = NewGameObject<GameObject>(L"test");
	test->transform.position = { 0,0,0 };
	test->transform.scale = { 0.1,0.1,0.1 };
	auto testInit = [this](SimpleMaterial* material)->void
		{
			material->SetVS(L"VertexSkinningShader.hlsl");				
			//material->SetVS(L"VertexShader.hlsl");				
			material->SetPS(L"PixelShader.hlsl");	
		};
	Utility::LoadFBX("Resource/Hip Hop Dancing.fbx", *test, nullptr, testInit, false);
	test->GetComponent<TransformAnimation>().PlayClip(L"mixamo.com");
	testList.push_back(test);
}

ResourceManagerScene::~ResourceManagerScene()
{
}

void ResourceManagerScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	cbuffer_Light& cb_Light = SimpleDirectionalLight::cb_Light;

	ImGui::Begin("Debug");
	ImGui::Text("vram : %zu MB", d3dRenderer.GetUsedVram());
	ImGui::Text("");

	if (mainCam)
	{
		ImGui::Text("Camera");
		ImGui::SliderFloat("FOV", &mainCam->FOV, 10, 120);
		ImGui::SliderFloat("CamSpeed", pCamSpeed, 1, 1000);
		ImGui::DragVector3("Cam Position", &mainCam->transform.position, 0);
		ImGui::DragQuaternion("Cam Rotation", &mainCam->transform.rotation, 0);
		ImGui::Text("");
	}

	ImGui::Text("Light");
	ImGui::DragFloat3("LightDir", (float*)&cb_Light.LightDir, 0.01f, -1.0f, 1.0f);
	ImGui::ColorEdit3("LightDiffuse", &cb_Light.LightDiffuse);
	ImGui::ColorEdit3("LightAmbient", &cb_Light.LightAmbient);
	ImGui::ColorEdit3("LightSpecular", &cb_Light.LightSpecular);
	ImGui::Text("");

	ImGui::Text("Test");
	if (ImGui::Button("Add Charater"))
		AddTestObject();
	if (ImGui::Button("Sub Charater"))
		SubTestObject();
	ImGui::Text("");

	ImGui::Text("Background");
	ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	ImGui::End();
}
void ResourceManagerScene::AddTestObject()
{
	auto obj = NewGameObject<GameObject>(L"test");
	if (!testList.empty())
	{
		obj->transform.position = testList.back()->transform.position;
		obj->transform.position += Vector3::Forward * -10;
	}
	obj->transform.scale = { 0.1,0.1,0.1 };

	auto testInit = [this](SimpleMaterial* material)->void
		{
			material->SetVS(L"VertexSkinningShader.hlsl");
			//material->SetVS(L"VertexShader.hlsl");				
			material->SetPS(L"PixelShader.hlsl");
		};
	Utility::LoadFBX("Resource/Hip Hop Dancing.fbx", *obj, nullptr, testInit, false);
	obj->GetComponent<TransformAnimation>().PlayClip(L"mixamo.com");

	testList.push_back(obj);
}
void ResourceManagerScene::SubTestObject()
{
	if (testList.size() == 0)
		return;

	GameObject* obj = testList.back();
	sceneManager.DestroyObject(obj);
	testList.erase(--testList.end());
}
#pragma warning(default : 4305)