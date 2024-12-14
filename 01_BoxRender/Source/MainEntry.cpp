#include "StartApp.h"

int WINAPI wWinMain(_In_ HINSTANCE hInst,
    _In_opt_ HINSTANCE hInstPrev,
    _In_ LPWSTR lpCmdLine,
    _In_ int cmdshow)
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