#include "TestMainScene.h"
#include <framework.h>

#include "../../11_IBL/Source/IBLTestScene.h"

TestMainScene::TestMainScene()
{
    D3D11_RASTERIZER_DESC rasterDesc{};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    d3dRenderer.SetRRState(rasterDesc);

	Scene::UseImGUI = true;
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
    mainCam->transform.position = Vector3(0.f, 15.f, -23.f);
    mainCam->transform.rotation = Vector3(-13.f, -21.f, 5.f);
    mainCam->AddComponent<CameraMoveHelper>();
    pCamSpeed = &mainCam->GetComponent<CameraMoveHelper>().moveSpeed;

    auto dancing = NewGameObject(L"HipHopDancing");
    Utility::LoadFBX(L"Resource/Stupid Bodyguard.fbx", *dancing, false, SURFACE_TYPE::BlingPhong);
    dancing->GetComponent<TransformAnimation>().PlayClip(L"Scene");
    dancing->transform.scale = Vector3(0.1f, 0.1f, 0.1f);

    auto chara = NewGameObject(L"char");
    auto initMeshChar = [this](MeshRender* mesh)
        {
            PBRMeshObject* pbrObj = static_cast<PBRMeshObject*>(&mesh->gameObject);
            charObjectList[pbrObj->GetNameToString()] = pbrObj;
        };

    Utility::LoadFBX(L"Resource/char/char.fbx", *chara, initMeshChar, false, SURFACE_TYPE::PBR);
    chara->transform.position = Vector3(10.0f, 0.f, 0.f);
    chara->transform.scale = Vector3(0.1f, 0.1f, 0.1f);

}


void TestMainScene::ImGUIRender()
{
    static bool showCharEdit = false;
    ImGui::Begin("Debug");
    {
        ImGui::Text("FPS : %d", TimeSystem::Time.GetFrameRate());
        ImGui::DragVector3("Camera Position", &mainCam->transform.position);
        ImGui::DragQuaternion("Camera Rotation", &mainCam->transform.rotation);
        ImGui::SliderFloat("Cam FOV", &camera->FOV, 10.f, 105.f);
        ImGui::SliderFloat("Camera Near", &camera->Near, 0.001f, 10.f);
        ImGui::SliderFloat("Camera Far", &camera->Far, 100.f, 10000.f);
        ImGui::SliderFloat("CamSpeed", pCamSpeed, 1, 1000);

        if (ImGui::Button("Char Editer"))
            showCharEdit = !showCharEdit;

        ImGui::DragFloat3("Light Dir", (float*)&SimpleDirectionalLight::cb_light.LightDir, 0.01f, -1.0f, 1.0f);

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

    ImGui::Begin("Lights");
    {
        ImGui::EditLight(&DirectionalLight::DirectionalLights);
    }
    ImGui::End();

    if (showCharEdit)
    {
        ImGui::Begin("Char Edit", &showCharEdit);
        int id = 0;
        for (auto& [key, object] : charObjectList)
        {
            ImGui::PushID(id);
            ImGui::Text(key.c_str());
            ImGui::Checkbox("Active", &object->Active);
            if (ImGui::Button("Wire Draw"))
            {
                for (int i = 0; i < object->GetComponentCount(); i++)
                {
                    MeshRender* mesh = object->GetComponentAtIndex<MeshRender>(i);
                    if (mesh)
                    {
                        mesh->SetFillMode(D3D11_FILL_WIREFRAME);
                    }
                }
            }
            if (ImGui::Button("Solid Draw"))
            {
                for (int i = 0; i < object->GetComponentCount(); i++)
                {
                    MeshRender* mesh = object->GetComponentAtIndex<MeshRender>(i);
                    if (mesh)
                    {
                        mesh->SetFillMode(D3D11_FILL_SOLID);
                    }
                }
            }
            ImGui::Text("\n");
            ImGui::PopID();
            id++;
        }
        ImGui::End();
    }
}

void TestMainScene::OnInputProcess(const DirectX::Keyboard::State& KeyState, const DirectX::Keyboard::KeyboardStateTracker& KeyTracker, const DirectX::Mouse::State& MouseState, const DirectX::Mouse::ButtonStateTracker& MouseTracker)
{
    if (KeyState.IsKeyDown(Keyboard::F1))
    {
        sceneManager.LoadScene<IBLTestScene>();
    }

    if (KeyState.IsKeyDown(Keyboard::Escape))
    {
        D3D11_GameApp::GameEnd();
    }
}