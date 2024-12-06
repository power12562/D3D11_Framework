#include "TestMainScene.h"
#include <Framework/SceneManager.h>
#include <GameObject/Base/CameraObject.h>
#include <Component/Camera/CameraMoveHelper.h>
#include <Utility/AssimpUtility.h>
#include <Material/BlingPhongMaterial.h>
#include <Component/TransformAnimation.h>
#include <Framework/ImguiHelper.h>
#include <Framework/TimeSystem.h>
#include <Light/SimpleDirectionalLight.h>
#include <Component/Render/MeshRender.h>
#include <Framework/ResourceManager.h>
#include <Utility/utfConvert.h>

TestMainScene::TestMainScene()
{
	Scene::UseImGUI = true;
    d3dRenderer.backgroundColor = { 1.f,1.f,1.f,1.f };

    material = GetResourceManager<cb_BlingPhongMaterial>().GetResource(L"BlingPhong");

    mainCam = NewGameObject<CameraObject>(L"Camera");
    camera = &mainCam->GetComponent<Camera>();
    camera->SetMainCamera();
    mainCam->transform.position = Vector3(10.f, 40.f, -63.f);
    mainCam->transform.rotation = Vector3(-13.f, -21.f, 5.f);
    mainCam->AddComponent<CameraMoveHelper>();
    pCamSpeed = &mainCam->GetComponent<CameraMoveHelper>().moveSpeed;

    auto HipHopDancing = NewGameObject(L"Hip Hop Dancing.fbx");
    auto SkinningMesh = [this](MeshRender* mesh)->void
        {
            int index = mesh->constBuffer.CreatePSConstantBuffers<cb_BlingPhongMaterial>();
            mesh->constBuffer.BindUpdateEvent(*material);

            index = mesh->constBuffer.CreatePSConstantBuffers<cb_DirectionalLight>();
            mesh->constBuffer.BindUpdateEvent(SimpleDirectionalLight::cb_light);

            mesh->SetVertexShader(L"VertexSkinningShader.hlsl");
            mesh->SetPixelShader(L"PixelShader.hlsl");
        };
    Utility::LoadFBX(L"Resource/Stupid Bodyguard.fbx", *HipHopDancing, SkinningMesh, false);
    HipHopDancing->GetComponent<TransformAnimation>().PlayClip(L"Scene");
    HipHopDancing->transform.scale = Vector3(0.1f, 0.1f, 0.1f);

    auto House = NewGameObject(L"char");
    auto objMesh = [this](MeshRender* mesh)
        {
            charObjectList[utfConvert::wstring_to_utf8(mesh->gameObject.Name).c_str()] = &mesh->gameObject;
            charMaterialList.emplace_back(cb_PBRMaterial());

            int index = mesh->constBuffer.CreatePSConstantBuffers<cb_PBRDirectionalLight>();
            mesh->constBuffer.BindUpdateEvent(pbrLight);

            cb_PBRMaterial& my_meterial = charMaterialList.back();
            my_meterial.baseColor = mesh->baseColor;
            index = mesh->constBuffer.CreatePSConstantBuffers<cb_PBRMaterial>();
            mesh->constBuffer.BindUpdateEvent(my_meterial);

            mesh->SetVertexShader(L"VertexShader.hlsl");
            mesh->SetPixelShader(L"PBRPixelShader.hlsl");
        };
    Utility::LoadFBX(L"Resource/char/char.fbx", *House, objMesh, false);
    House->transform.position = Vector3(10.0f, 0.f, 0.f);
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
        ImGui::SliderFloat("Camera Near", &camera->Near, 0.001f, 10.f);
        ImGui::SliderFloat("Camera Far", &camera->Far, 100.f, 10000.f);
        ImGui::SliderFloat("CamSpeed", pCamSpeed, 1, 1000);

        ImGui::DragFloat3("Light Dir", (float*)&SimpleDirectionalLight::cb_light.LightDir, 0.01f, -1.0f, 1.0f);
        pbrLight.LightDir = SimpleDirectionalLight::cb_light.LightDir;

        ImGui::ColorEdit4("Bg Color", &d3dRenderer.backgroundColor);
    }
    ImGui::End();
}
