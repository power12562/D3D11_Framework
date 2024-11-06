#include "TestApp.h"
#include <GameObject/Base/CameraObject.h>
#include <Math/Mathf.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <Framework\SceneManager.h>

#include "../Source/TestCubeComponent.h"
#include "../Source/TestMainScene.h"


int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    TestApp app;
    app.Initialize(hInst);
    sceneManager.LoadScene<TestMainScene>();
    app.Run();             
    app.Uninitialize();

    return 0;
}