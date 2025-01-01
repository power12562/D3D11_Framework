#include "TestMainScene.h"
#include <framework.h>
#include "TestAppManagerObject.h"

TestMainScene::TestMainScene()
{
    Scene::UseImGUI = true;
    GuizmoSetting.UseImGuizmo = true;

    D3D11_RASTERIZER_DESC rasterDesc{};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    d3dRenderer.SetRRState(rasterDesc);

    d3dRenderer.backgroundColor = { 1.f,1.f,1.f,1.f };

    material = GetResourceManager<cb_BlingPhongMaterial>().GetResource(L"BlingPhong");
}

TestMainScene::~TestMainScene()
{
}

void TestMainScene::Start()
{
    mainCam = NewGameObject<CameraObject>(L"Camera");
    camera = &mainCam->GetComponent<Camera>();
    camera->SetMainCamera();
    mainCam->transform.position = Vector3(70.f, 80.f, -200.f);
    mainCam->AddComponent<CameraMoveHelper>();
    pCamSpeed = &mainCam->GetComponent<CameraMoveHelper>().moveSpeed;

    chara = NewGameObject(L"char");
    Utility::LoadFBX(L"Resource/char/char.fbx", *chara, false, SURFACE_TYPE::PBR);

    auto Manager = NewGameObject<TestAppManagerObject>(L"TestAppManager");
    GameObject::DontDestroyOnLoad(Manager);
}


void TestMainScene::ImGUIRender()
{
    ImGui::Begin("Debug Window");
    {
        ImGui::Text("FPS : %d", TimeSystem::Time.GetFrameRate());
        ImGui::DragVector3("Camera Position", &mainCam->transform.position);
        ImGui::DragQuaternionWorld("Camera Rotation", &mainCam->transform.rotation);
        ImGui::SliderFloat("Cam FOV", &camera->FOV, 10.f, 105.f);
        ImGui::SliderFloat("Camera Near", &camera->Near, 0.001f, 10.f);
        ImGui::SliderFloat("Camera Far", &camera->Far, 100.f, 10000.f);
        ImGui::SliderFloat("CamSpeed", pCamSpeed, 1, 1000);

        ImGui::DragFloat3("Light Dir", (float*)&SimpleDirectionalLight::cb_light->LightDir, 0.01f, -1.0f, 1.0f);

        ImGui::ColorEdit4("Bg Color", &d3dRenderer.backgroundColor);

        if (ImGui::Button("Change Resolution"))
            ImGui::OpenPopup("Screen Resolution");

        if (ImGui::BeginPopupModal("Screen Resolution", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static SIZE resolution;
            // 해상도 입력
            ImGui::InputInt("Width", reinterpret_cast<int*>(&resolution.cx));
            ImGui::InputInt("Height", reinterpret_cast<int*>(&resolution.cy));

            if (ImGui::Button("OK"))
            {
                if (resolution.cx > 0 && 0 < resolution.cy)
                {
                    D3D11_GameApp::ChangeResolution(resolution);
                    resolution.cx = 0;
                    resolution.cy = 0;
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();

    ImGui::Begin("D3DRenderer");
    {
        ImGui::EditD3DRenderer();
    }
    ImGui::End();

    ImGui::Begin("Lights");
    {
        ImGui::EditLight(DirectionalLight::DirectionalLights.get());
    }
    ImGui::End();

    ImGui::Begin("Hierarchy View");
    {
        ImGui::EditHierarchyView();
    }
    ImGui::End();
}
