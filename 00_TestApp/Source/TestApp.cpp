#include "TestApp.h"
#include <framework.h>

TestApp::TestApp()
{
    this->windowName = L"TestApp";

    this->SetBorderlessWindowed();
    //this->SetOptimalScreenSize();
}

TestApp::~TestApp()
{

}

void TestApp::PreProcessMouseMessage(UINT message, MouseVK vk)
{
    switch (message)
    {
	case WM_LBUTTONDOWN:
		Debug_printf("mouse : LButton %d\n", (int)vk);
		break;
	case WM_RBUTTONDOWN:
		Debug_printf("mouse : RButton %d\n", (int)vk);
		break;
	case WM_MBUTTONDOWN:
		Debug_printf("mouse : MButton %d\n", (int)vk);
		break;
	case WM_XBUTTONDOWN:
		Debug_printf("mouse : XButton %d\n", (int)vk);
		break;
	default:
		break;
    }    
}

void TestApp::PreProcessKeyboradMessage(UINT message, KeyboardVK vk)
{
	static bool stay[255]{};
	int key = (int)vk;
	if (!stay[key] && WM_KEYDOWN == message)
	{
		Debug_printf("keyboread Down : %d\n", key);
		stay[key] = true;
	}
	else if (WM_KEYUP == message)
	{
		//Debug_printf("keyboread Up : %d\n", key);
		stay[key] = false;
	}
}

