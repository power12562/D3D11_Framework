#include "BlinnPhongTestApp.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    BlinnPhongTestApp app;
    app.Initialize(hInst);
    app.InitScene();
    app.InitImGUI();
    app.Run();
    app.UninitImGUI();
    app.UninitScene();
    app.Uninitialize();

    return 0;
}