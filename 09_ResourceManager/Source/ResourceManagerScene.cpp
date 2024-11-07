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
#include <Framework/TimeSystem.h>

#pragma warning(disable : 4305)
ResourceManagerScene::ResourceManagerScene()
{
	UseImGUI = true;

	//리소스 미리 로드
	Utility::LoadFBXResource(L"Resource/Hip Hop Dancing.fbx");

	SimpleDirectionalLight::cb_Light.LightDir = { 0.5, 0, 1, 0 };

	d3dRenderer.backgroundColor = Color(0, 0, 0, 1);

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0, 30, -25);
	cam->transform.rotation = Vector3(-15, 0, 0);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	//for (size_t i = 0; i < 100; i++) { AddTestObject(); }
}

ResourceManagerScene::~ResourceManagerScene()
{
}

void ResourceManagerScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	cbuffer_Light& cb_Light = SimpleDirectionalLight::cb_Light;

	ImGui::Begin("Debug");
	{
		ImGui::Text("vram : %zu MB	Obj Count : %zu  FPS : %d", d3dRenderer.GetUsedVram(), testList.size(), TimeSystem::Time.GetFrameRate());
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
		if (ImGui::Button("Clear Charater"))
			ClearTestObject();
		ImGui::Text("");

		ImGui::Text("Background");
		ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	}
	ImGui::End();
}
void ResourceManagerScene::AddTestObject()
{
	auto obj = NewGameObject<GameObject>(L"test");
	if (!testList.empty())
	{
		obj->transform.position = testList.back()->transform.position;
		if (std::abs(obj->transform.position.x - 30.f) <= Mathf::Epsilon)
		{
			obj->transform.position += Vector3::Left * 60;
			obj->transform.position += Vector3::Backward * 10;
		}
		else
		{
			obj->transform.position += Vector3::Right * 10;
		}
	}
	else
	{
		obj->transform.position = Vector3(-30, 0, 0);
	}
	obj->transform.scale = { 0.1,0.1,0.1 };

	auto testInit = [this](SimpleMaterial* material)->void
		{
			material->SetVS(L"VertexSkinningShader.hlsl");			
			material->SetPS(L"PixelShader.hlsl");
		};
	Utility::LoadFBX(L"Resource/Hip Hop Dancing.fbx", *obj, nullptr, testInit, false);
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
void ResourceManagerScene::ClearTestObject()
{
	while (!testList.empty())
	{
		SubTestObject();
	}
}
#pragma warning(default : 4305)