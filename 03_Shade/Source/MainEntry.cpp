#include "ShadeTestApp.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    ShadeTestApp app;
    app.Initialize(hInst);
    app.InitScene();
    app.InitImGUI();
    app.Run();
    app.UninitScene();
    app.UninitImGUI();
    app.Uninitialize();

    return 0;
}