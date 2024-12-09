#include "StartApp.h"
#include <Framework\SceneManager.h>

#include "..\Source\SkinningAnimationScene.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    SkinningAnimationApp app;
    app.Initialize(hInst);
    sceneManager.LoadScene<SkinningAnimationScene>();
    app.Run();
    app.Uninitialize();

    return 0;
}