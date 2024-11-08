#include "TestMainScene.h"
#include <Framework/SceneManager.h>
#include <GameObject/Base/CameraObject.h>
#include <Component/Camera/CameraMoveHelper.h>
#include <Utility/AssimpUtility.h>
#include <Material/SimpleMaterial.h>
#include <Component/TransformAnimation.h>
#include <Framework/ImguiHelper.h>
#include <Framework/TimeSystem.h>

TestMainScene::TestMainScene()
{
	Scene::UseImGUI = true;

    auto mainCam = NewGameObject<CameraObject>(L"Camera");
    mainCam->GetComponent<Camera>().SetMainCamera();
    mainCam->transform.position += Vector3(0, 3, 0);
    mainCam->AddComponent<CameraMoveHelper>();

    auto HipHopDancing = NewGameObject(L"Hip Hop Dancing.fbx");
    auto initMaterial = [this](SimpleMaterial* material)->void
        {
            material->SetVS(L"VertexSkinningShader.hlsl");
            material->SetPS(L"PixelShader.hlsl");
        };
    Utility::LoadFBX(L"Resource/Stupid Bodyguard.fbx", *HipHopDancing, nullptr, initMaterial, false);
    HipHopDancing->GetComponent<TransformAnimation>().PlayClip(L"Scene");
    HipHopDancing->transform.scale = Vector3(0.1f, 0.1f, 0.1f);
}

TestMainScene::~TestMainScene()
{
}

void TestMainScene::ImGUIRender()
{
    ImGui::Begin("Debug");
    {
        ImGui::Text("FPS : %d", TimeSystem::Time.GetFrameRate());
    }
    ImGui::End();
}
