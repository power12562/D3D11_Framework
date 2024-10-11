#include "ImportFBXScene.h"
#include <Framework\SceneManager.h>
#include <Component\CameraMoveHelper.h>
#include <Framework/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <Framework\ImguiHelper.h>
#include <Math\Mathf.h>

#include "../Source/AssimpComponent.h"
#include "../Source/SimpleDirectionalLight.h"

ImportFBXScene::ImportFBXScene()
{
	UseImGUI = true;
	d3dRenderer.backgroundColor = Color(1, 1, 1, 1);
	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->AddComponent<SimpleDirectionalLight>();
	cam->transform.position = Vector3(0.98, 13.835, -16);
	cam->transform.rotation = Quaternion::CreateFromYawPitchRoll( Vector3(-16.45, 0.15, -0.04) * Mathf::Deg2Rad );
	cam->AddComponent<CameraMoveHelper>();

	auto obj = NewGameObject<GameObject>(L"obj");
	obj->transform.scale = { 0.1,0.1,0.1 };
	obj->AddComponent<AssimpComponent>();
}

ImportFBXScene::~ImportFBXScene()
{
}

void ImportFBXScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();
	cbuffer_Light& cb_Light = SimpleDirectionalLight::cb_Light;
	cbuffer_bool& cb_bool = SimpleDirectionalLight::cb_bool;

	ImGui::Begin("Debug");
	ImGui::Text("Camera");
	ImGui::SliderFloat("FOV", &mainCam->FOV, 10, 120);
	ImGui::DragVector3("Cam Position", &mainCam->transform.position, 0);  
	ImGui::DragQuaternion("Cam Rotation", &mainCam->transform.rotation, 0);
	ImGui::Text("");

	ImGui::Text("Light");
	ImGui::DragFloat3("LightDir", (float *)&cb_Light.LightDir, 0.01f, -1.0f, 1.0f);
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
