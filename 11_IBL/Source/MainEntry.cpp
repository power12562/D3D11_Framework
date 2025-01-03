#include "StartApp.h"
#include <Manager/SceneManager.h>

#include "../Source/IBLTestScene.h"

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    IBLTestApp app;

    app.Initialize(hInstance);
    sceneManager.LoadScene<IBLTestScene>();
    app.Run();
    app.Uninitialize();

    return 0;
}