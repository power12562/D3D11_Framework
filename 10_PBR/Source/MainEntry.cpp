#include "StartApp.h"
#include <Framework\SceneManager.h>
#include "..\Source\PBRTestScene.h"

//entry point
int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nCmdShow);

	ImportFBXapp app;
	app.Initialize(hInstance);
	sceneManager.LoadScene<PBRTestScene>();
	app.Run();
	app.Uninitialize();

    return 0;
}