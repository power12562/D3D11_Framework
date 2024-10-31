#include "TestApp.h"
#include <GameObject/Base/CameraObject.h>
#include <Math/Mathf.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <Framework\SceneManager.h>

#include "../Source/TestCubeComponent.h"


int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    TestApp app;
    app.Initialize(hInst);

    auto mainCam = NewGameObject<CameraObject>(L"Camera");
    mainCam->GetComponent<Camera>().SetMainCamera();
    mainCam->transform.position += Vector3(0, 3, 0);
    mainCam->AddComponent<CameraMoveHelper>();

    auto cube = NewGameObject<GameObject>(L"A");
    cube->AddComponent<TestCubeComponent>();
    cube->transform.position += Vector3(0, 0, 5);

    auto cube2 = NewGameObject<GameObject>(L"B");
    cube2->AddComponent<TestCubeComponent>();
    cube2->transform.position += Vector3(2.5, 0, 5);
    cube2->transform.SetParent(cube->transform);

    auto cube3 = NewGameObject<GameObject>(L"C");
    cube3->AddComponent<TestCubeComponent>();
    cube3->transform.position += Vector3(5, 0, 5);
    cube3->transform.SetParent(cube2->transform);

    app.Run();
    app.Uninitialize();

    return 0;
}