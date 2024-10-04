#include "ShadeTestApp.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    ShadeTestApp app;
    app.Initialize(hInst);
    app.InitScene();
    app.Run();
    app.UninitScene();
    app.Uninitialize();

    return 0;
}