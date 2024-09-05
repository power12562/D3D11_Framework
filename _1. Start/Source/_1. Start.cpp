#include "_1. Start.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
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
