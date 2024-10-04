#include "MappingTestScene.h"
#include <Framework/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Math/Mathf.h>

#include "../Source/LightManager.h"
#include "../Source/MappingCubeObject.h"

MappingTestScene::MappingTestScene()
{
	UseImGUI = true;

	auto mainCam = new CameraObject(L"MainCamera");
	mainCam->SetMainCamera();
	mainCam->AddComponent<LightManager>();

	auto cube1 = new GameObject(L"cube1");
	cube1->AddComponent<MappingCubeObject>();
	cube1->transform.position = Vector3{ 0,0,3 };
}

MappingTestScene::~MappingTestScene()
{

}

void MappingTestScene::ImGUIRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Camera* mainCam = Camera::GetMainCamera();
	cbuffer_Light& cb_Light = LightManager::cb_Light;
	cbuffer_bool& cb_bool = LightManager::cb_bool;

	ImGui::Begin("Debug");
	ImGui::Text("Camera");
	ImGui::DragFloat3("Cam Position", camPos);
	mainCam->transform.position = Vector3(camPos);
	ImGui::DragFloat3("Cam Rotation", camRotation);
	mainCam->transform.rotation = Quaternion::CreateFromYawPitchRoll(Vector3(camRotation) * Mathf::Deg2Rad);
	ImGui::SliderFloat("FOV", &mainCam->FOV, 10, 120);
	ImGui::Text("");
	ImGui::Text("Cube");
	ImGui::DragFloat3("Cube Rotation", cubeRotation);
	objectList[1]->transform.rotation = Quaternion(Quaternion::CreateFromYawPitchRoll(Vector3(cubeRotation) * Mathf::Deg2Rad));
	ImGui::SliderFloat("Cube Scale", &cubeScale, 1.0f, 10.0f);
	objectList[1]->transform.scale = Vector3(cubeScale, cubeScale, cubeScale);
	ImGui::Text("");
	ImGui::Text("Light");	
	ImGui::DragFloat3("LightDir", LightDir, 0.01f, -1.0f, 1.0f);
	cb_Light.LightDir = Vector4(LightDir);
	ImGui::ColorEdit3("LightDiffuse", LightDiffuse);
	cb_Light.LightDiffuse = Vector4(LightDiffuse);
	ImGui::ColorEdit3("LightAmbient", LightAmbient);
	cb_Light.LightAmbient = Vector4(LightAmbient);
	ImGui::ColorEdit3("LightSpecular", LightSpecular);
	cb_Light.LightSpecular = Vector4(LightSpecular);
	ImGui::Text("");
	ImGui::Text("Material");
	ImGui::Checkbox("Use NormalMap", (bool*)& cb_bool.UseNormalMap);
	ImGui::Checkbox("Use SpecularMap", (bool*)&cb_bool.UseSpecularMap);
	ImGui::ColorEdit3("MaterialDiffuse", MaterialDiffuse);
	cb_Light.MaterialDiffuse = Vector4(MaterialDiffuse);
	ImGui::ColorEdit3("MaterialAmbient", MaterialAmbient);
	cb_Light.MaterialAmbient = Vector4(MaterialAmbient);
	ImGui::ColorEdit3("MaterialSpecular", MaterialSpecular);
	cb_Light.MaterialSpecular = Vector4(MaterialSpecular);
	ImGui::DragFloat("MaterialSpecularPower", &cb_Light.MaterialSpecularPower, 10, 50, 500);
	ImGui::Text("");
	ImGui::Text("Background");
	ImGui::ColorEdit3("BgColor", bgColor);
	d3dRenderer.backgroundColor = Color(bgColor);
	ImGui::End();
}
