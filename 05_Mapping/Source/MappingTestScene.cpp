#include "MappingTestScene.h"
#include <D3DCore/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <Utility/ImguiHelper.h>
#include <Math/Mathf.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <Manager/SceneManager.h>

#include "../Source/LightManager.h"
#include "../Source/MappingCubeObject.h"

MappingTestScene::MappingTestScene()
{
	UseImGUI = true;

	auto mainCam = NewGameObject<CameraObject>(L"MainCamera");
	mainCam->SetMainCamera();
	mainCam->AddComponent<LightManager>();
	mainCam->AddComponent<CameraMoveHelper>();

	cube1 = NewGameObject<GameObject>(L"cube1");
	cube1->AddComponent<MappingCubeObject>();
	cube1->transform.position = Vector3{ 0,0,3 };
}

MappingTestScene::~MappingTestScene()
{

}

void MappingTestScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	cb_DirectionalLight& cb_light = LightManager::cb_light;
	cbuffer_bool& cb_bool = LightManager::cb_bool;

	ImGui::Begin("Debug");
	ImGui::Text("Camera");
	ImGui::SliderFloat("FOV", &mainCam->FOV, 10, 120);
	ImGui::Text("");
	ImGui::Text("Cube");	
	ImGui::DragVector3("Cube Position", &cube1->transform.position);
	ImGui::DragQuaternion("Cube Rotation", &cube1->transform.rotation);
	ImGui::SliderFloat("Cube Scale", &cubeScale, 1.0f, 10.0f);
	cube1->transform.scale = Vector3(cubeScale, cubeScale, cubeScale);
	ImGui::Text("");
	ImGui::Text("Light");	
	ImGui::DragFloat3("LightDir", LightDir, 0.01f, -1.0f, 1.0f);
	cb_light.LightDir = Vector4(LightDir);
	ImGui::ColorEdit3("LightDiffuse", &cb_light.LightColor);
	ImGui::ColorEdit3("LightAmbient", &cb_light.LightAmbient);
	ImGui::ColorEdit3("LightSpecular", &cb_light.LightSpecular);
	ImGui::Text("");
	ImGui::Text("SimpleMaterial");
	ImGui::Checkbox("Use NormalMap", &cb_bool.UseNormalMap);
	ImGui::Checkbox("Use SpecularMap", &cb_bool.UseSpecularMap);
	ImGui::ColorEdit3("MaterialDiffuse", &cb_light.MaterialDiffuse);
	ImGui::ColorEdit3("MaterialAmbient", &cb_light.MaterialAmbient);
	ImGui::ColorEdit3("MaterialSpecular", &cb_light.MaterialSpecular);
	ImGui::DragFloat("MaterialSpecularPower", &cb_light.MaterialSpecularPower, 10, 50, 500);
	ImGui::Text("");
	ImGui::Text("Background");
	ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	ImGui::End();
}
