#include "PBRTestScene.h"
#include <Framework\SceneManager.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <Framework/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>
#include <Framework\ImguiHelper.h>
#include <Light\SimpleDirectionalLight.h>
#include <Utility\AssimpUtility.h>
#include <Framework/TimeSystem.h>
#include <Component/Render/MeshRender.h>
#include <Framework/ResourceManager.h>

#include <Material/PBRMaterial.h>

#pragma warning(disable : 4305)
PBRTestScene::PBRTestScene()
{
	UseImGUI = true;

	d3dRenderer.backgroundColor = Color(0, 0, 0, 1);

	ID3D11RasterizerState* pRRState;
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK; 
	rasterDesc.FrontCounterClockwise = false; 
	d3dRenderer.CreateRRState(rasterDesc, &pRRState);
	d3dRenderer.SetRRState(pRRState);
	Utility::SafeRelease(pRRState);

	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->transform.position = Vector3(0.f, 8.f, -20.f);
	cam->transform.rotation = Vector3(0.f, 0.f, 0.f);
	pCamSpeed = &cam->AddComponent<CameraMoveHelper>().moveSpeed;

	shared_material = GetResourceManager<cb_PBRMaterial>().GetResource(L"PBR");
	auto initCerberusShader = [this](MeshRender* mesh)
		{ 
			int index = mesh->constBuffer.CreatePSConstantBuffers<cb_Light>();
			mesh->constBuffer.BindUpdateEvent(SimpleDirectionalLight::cb_light);

			mesh->SetVertexShader(L"Shader/PBRVertexShader.hlsl");
			mesh->SetPixelShader(L"Shader/PBRPixelShader.hlsl");
		};
	auto cerberus = NewGameObject(L"cerberus");
	Utility::LoadFBX(L"Resource/cerberus/cerberus.fbx", *cerberus, initCerberusShader, false);
	cerberus->transform.position += Vector3::Left * 10.f;
	cerberus->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };

	auto initCharShader = [this](MeshRender* mesh)
		{
			int index = mesh->constBuffer.CreatePSConstantBuffers<cb_Light>();
			mesh->constBuffer.BindUpdateEvent(SimpleDirectionalLight::cb_light);

			charMaterialList[mesh->gameObject.Name] = cb_PBRMaterial(mesh->baseColor);
			index = mesh->constBuffer.CreatePSConstantBuffers<cb_PBRMaterial>();
			mesh->constBuffer.BindUpdateEvent(charMaterialList[mesh->gameObject.Name]);

			mesh->SetVertexShader(L"Shader/PBRVertexShader.hlsl");
			mesh->SetPixelShader(L"Shader/PBRPixelShader.hlsl");
		};
	auto charater = NewGameObject(L"charater");
	Utility::LoadFBX(L"Resource/char/char.fbx", *charater, initCharShader, false);
	charater->transform.position += Vector3::Right * 10.f;
	charater->transform.rotation = Vector3::Up * 23.f;
	charater->transform.scale = Vector3{ 0.1f, 0.1f, 0.1f };
}

PBRTestScene::~PBRTestScene()
{
}

void PBRTestScene::ImGUIRender()
{
	Camera* mainCam = Camera::GetMainCamera();

	ImGui::Begin("Debug");
	{
		ImGui::Text("FPS : %d", TimeSystem::Time.GetFrameRate());

		if (mainCam)
		{
			ImGui::Text("Camera");
			ImGui::SliderFloat("FOV", &mainCam->FOV, 10, 120);
			ImGui::SliderFloat("CamNear", &mainCam->Near, 0.05f, 10.f);
			ImGui::SliderFloat("CamFar", &mainCam->Far, 100.f, 10000.f);
			ImGui::SliderFloat("CamSpeed", pCamSpeed, 1, 1000);
			ImGui::DragVector3("Cam Position", &mainCam->transform.position, 0);
			ImGui::DragQuaternion("Cam Rotation", &mainCam->transform.rotation, 0);
			ImGui::Text("");
		}

		ImGui::Text("Material");
		
		ImGui::Text("Background");
		ImGui::ColorEdit3("BgColor", &d3dRenderer.backgroundColor);
	}
	ImGui::End();
}
