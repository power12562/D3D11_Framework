#include "TestApp.h"
#include <GameObject/Base/CameraObject.h>
#include <Math/Mathf.h>
#include <Component\CameraMoveHelper.h>

#include "../Source/TestCubeComponent.h"


int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    TestApp app;
    app.Initialize(hInst);

    auto mainCam = new CameraObject(L"Camera");
    mainCam->GetComponent<Camera>().SetMainCamera();
    mainCam->transform.position += Vector3(0, 3, 0);
    mainCam->AddComponent<CameraMoveHelper>();

    auto cube = new GameObject(L"Cube");
    cube->AddComponent<TestCubeComponent>();
    cube->transform.position += Vector3(0, 0, 5);

    auto cube2 = new GameObject(L"Cube1");
    cube2->AddComponent<TestCubeComponent>();
    cube2->transform.position += Vector3(2.5, 0, 5);
    cube2->transform.SetParent(cube->transform);

    app.Run();
    app.Uninitialize();

    return 0;
}