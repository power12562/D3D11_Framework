#include "MappingTestApp.h"
#include <GameObject\Base\CameraObject.h>

#include "../Source/LightManager.h"
#include "../Source/MappingCubeObject.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    MappingTestApp app;
    app.Initialize(hInst);

    d3dRenderer.backgroundColor = { 0.3,0.3,0.3,1 };

    auto mainCam = new CameraObject(L"MainCamera");
    mainCam->SetMainCamera();
    mainCam->AddComponent<LightManager>();

    auto cube1 = new GameObject(L"cube1");
    cube1->AddComponent<MappingCubeObject>();
    cube1->transform.position = Vector3{ 0,0,5 };

    app.Run();
    app.Uninitialize();

    return 0;
}