#include "TestApp.h"

TestApp::TestApp()
{
    this->windowName = L"TestApp";

    this->SetBorderlessWindowed();
    this->SetOptimalScreenSize();
}

TestApp::~TestApp()
{

}

