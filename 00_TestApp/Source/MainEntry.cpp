#include "TestApp.h"
#include <GameObject/Base/CameraObject.h>
#include <Math/Mathf.h>
#include <Component\Camera\CameraMoveHelper.h>
#include <Manager/SceneManager.h>

#include "../Source/TestMainScene.h"

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hInstPrev, _In_ LPWSTR cmdline, _In_ int cmdshow)
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