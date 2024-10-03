#include "TestApp.h"
#include <GameObject/CameraObject.h>
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
    mainCam->transform.rotation = Vector3(-15, 0, 0);

    auto cube = new GameObject(L"Cube");
    cube->AddComponent<TestCubeComponent>();
    cube->transform.position += Vector3(0, 0, 5);

    app.Run();
    app.Uninitialize();

    return 0;
}