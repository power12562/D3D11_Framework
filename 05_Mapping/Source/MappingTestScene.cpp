#include "MappingTestScene.h"
#include <Framework/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <Framework\ImguiHelper.h>
#include <Math/Mathf.h>
#include <Component\CameraMoveHelper.h>
#include <Framework\SceneManager.h>

#include "../Source/LightManager.h"
#include "../Source/MappingCubeObject.h"

MappingTestScene::MappingTestScene()
{
	UseImGUI = true;

	auto mainCam = NewGameObject<CameraObject>(L"MainCamera");
	mainCam->SetMainCamera();
	mainCam->AddComponent<LightManager>();
	mainCam->AddComponent<CameraMoveHelper>();

	auto cube1 = NewGameObject<GameObject>(L"cube1");
	cube1->AddComponent<MappingCubeObject>();
	cube1->transform.position = Vector3{ 0,0,3 };
}

MappingTestScene::~MappingTestScene()
{

}

void MappingTestScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	cbuffer_Light& cb_Light = LightManager::cb_Light;
	cbuffer_bool& cb_bool = LightManager::cb_bool;

	ImGui::Begin("Debug");
	ImGui::Text("Camera");
	ImGui::SliderFloat("FOV", &mainCam->FOV, 10, 120);
	ImGui::Text("");
	ImGui::Text("Cube");	
	ImGui::DragQuaternion("Cube Rotation", &objectList[1]->transform.rotation);
	ImGui::SliderFloat("Cube Scale", &cubeScale, 1.0f, 10.0f);
	objectList[1]->transform.scale = Vector3(cubeScale, cubeScale, cubeScale);
	ImGui::Text("");
	ImGui::Text("Light");	
	ImGui::DragFloat3("LightDir", LightDir, 0.01f, -1.0f, 1.0f);
	cb_Light.LightDir = Vector4(LightDir);
	ImGui::ColorEdit3("LightDiffuse", &cb_Light.LightDiffuse);
	ImGui::ColorEdit3("LightAmbient", &cb_Light.LightAmbient);
	ImGui::ColorEdit3("LightSpecular", &cb_Light.LightSpecular);
	ImGui::Text("");
	ImGui::Text("Material");
	ImGui::Checkbox("Use NormalMap", &cb_bool.UseNormalMap);
	ImGui::Checkbox("Use SpecularMap", &cb_bool.UseSpecularMap);
	ImGui::ColorEdit3("MaterialDiffuse", &cb_Light.MaterialDiffuse);
	ImGui::ColorEdit3("MaterialAmbient", &cb_Light.MaterialAmbient);
	ImGui::ColorEdit3("MaterialSpecular", &cb_Light.MaterialSpecular);
	ImGui::DragFloat("MaterialSpecularPower", &cb_Light.MaterialSpecularPower, 10, 50, 500);
	ImGui::Text("");
	ImGui::Text("Background");
	ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	ImGui::End();
}
