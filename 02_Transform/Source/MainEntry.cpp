#include "TransformTest.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    TransformTest app;
    app.Initialize(hInst);
    if(app.InitD3D() && app.InitScene() && app.InitImGUI())
    {
        app.Run();
    }
    app.UninitD3D();
    app.UninitScene();
    app.UninitImGUI();
    app.Uninitialize();
    return 0;
}