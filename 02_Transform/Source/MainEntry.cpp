#include "TransformTest.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    TransformTest app;
    app.Initialize(hInst);
    app.Run();
    app.Uninitialize();

    return 0;
}