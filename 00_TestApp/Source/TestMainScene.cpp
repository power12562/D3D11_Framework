#include "TestMainScene.h"
#include <Framework/SceneManager.h>
#include <GameObject/Base/CameraObject.h>
#include <Component/Camera/CameraMoveHelper.h>
#include <Utility/AssimpUtility.h>
#include <Material/SimpleMaterial.h>
#include <Component/TransformAnimation.h>
#include <Framework/ImguiHelper.h>
#include <Framework/TimeSystem.h>
#include <Light/SimpleDirectionalLight.h>
#include <Component/Render/MeshRender.h>

TestMainScene::TestMainScene()
{
	Scene::UseImGUI = true;
    d3dRenderer.backgroundColor = { 1.f,1.f,1.f,1.f };

    mainCam = NewGameObject<CameraObject>(L"Camera");
    mainCam->GetComponent<Camera>().SetMainCamera();
    mainCam->transform.position = Vector3(10.f, 40.f, -63.f);
    mainCam->transform.rotation = Vector3(-13.f, -21.f, 5.f);
    mainCam->AddComponent<CameraMoveHelper>();

    auto HipHopDancing = NewGameObject(L"Hip Hop Dancing.fbx");
    auto SkinningMesh = [this](MeshRender* mesh)->void
        {
            mesh->SetVertexShader(L"VertexSkinningShader.hlsl");
            mesh->SetPixelShader(L"PixelShader.hlsl");
        };
    Utility::LoadFBX(L"Resource/Stupid Bodyguard.fbx", *HipHopDancing, nullptr, SkinningMesh, false);
    HipHopDancing->GetComponent<TransformAnimation>().PlayClip(L"Scene");
    HipHopDancing->transform.scale = Vector3(0.1f, 0.1f, 0.1f);

    auto House = NewGameObject(L"House");
    auto objMesh = [this](MeshRender* mesh)
        {
            mesh->SetVertexShader(L"VertexShader.hlsl");
            mesh->SetPixelShader(L"PixelShader.hlsl");
        };
    Utility::LoadFBX(L"Resource/house.fbx", *House, nullptr, objMesh, true);
    House->transform.position = Vector3(50.0f, 0.f, 0.f);
    House->transform.scale = Vector3(0.1f, 0.1f, 0.1f);
}

TestMainScene::~TestMainScene()
{
}

void TestMainScene::ImGUIRender()
{
    ImGui::Begin("Debug");
    {
        ImGui::Text("FPS : %d", TimeSystem::Time.GetFrameRate());
        ImGui::DragVector3("Camera Position", &mainCam->transform.position);
        ImGui::DragQuaternion("Camera Rotation", &mainCam->transform.rotation);
        ImGui::DragFloat3("Light Dir", (float*)&SimpleDirectionalLight::cb_light.LightDir, 0.01f, -1.0f, 1.0f);
        ImGui::ColorEdit4("Bg Color", &d3dRenderer.backgroundColor);
    }
    ImGui::End();
}
