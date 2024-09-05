#include "01_Start.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, int cmdshow)
{
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdshow);

    StartApp app;
    app.Initialize(hInst);
    app.Run();
    app.Uninitialize();

    return 0;
}

StartApp::StartApp()
{
    this->windowName = L"StartApp";
   

}

StartApp::~StartApp()
{
}

void StartApp::Update()
{
}

void StartApp::Render()
{
}
