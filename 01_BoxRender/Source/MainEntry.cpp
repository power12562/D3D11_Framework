#include "StartApp.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    D3D11_BoxDrawApp app;
    app.Initialize(hInst);
    app.InitD3D();
    app.InitScene();
    app.Run();
    app.Uninitialize();

    return 0;
}