#include "DeferredTestApp01.h"
#include <DeferredTestScene1.h>

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    DeferredTestApp01 app;
    app.Initialize(hInstance);
    sceneManager.LoadScene<DeferredTestScene1>();
    app.Run();
    app.Uninitialize();

    return 0;
}