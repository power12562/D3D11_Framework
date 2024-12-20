#include "StartApp.h"
#include <Manager/SceneManager.h>

#include "..\Source\ResourceManagerScene.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

	ResourceManagerApp app;
	app.Initialize(hInst);
	sceneManager.LoadScene<ResourceManagerScene>();
	app.Run();
	app.Uninitialize();

    return 0;
}