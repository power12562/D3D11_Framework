#include "MappingTestApp.h"
#include <GameObject\Base\CameraObject.h>
#include <Framework/SceneManager.h>

#include "../Source/MappingTestScene.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    MappingTestApp app;
    app.Initialize(hInst);

    sceneManager.LoadScene<MappingTestScene>();

    app.Run();
    app.Uninitialize();

    return 0;
}