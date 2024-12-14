#include "TransformTest.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nShowCmd);

    TransformTest app;
    app.Initialize(hInstance);
    if(app.InitD3D() && app.InitScene())
    {
        app.Run();
    }
    app.UninitD3D();
    app.UninitScene();
    app.Uninitialize();
    return 0;
}