#include "WinGameApp.h"

#include <Framework\D3DRenderer.h>
#include <Framework/TimeSystem.h>
#include <Framework/ThreadManager.h>
#include <cassert>
#include <cstdio>
#include <clocale>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <Framework/ImguiHelper.h>

LRESULT CALLBACK DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void WinGameApp::Initialize(HINSTANCE hinstance)
{
	if (RunApp != nullptr)
	{
		__debugbreak(); //������ �ʱ�ȭ �Ұ���.
		return;
	}
		
	setlocale(LC_ALL, "");
#ifdef _DEBUG
	AllocConsole();
	FILE* _tempFile;
	freopen_s(&_tempFile, "CONOUT$", "w", stdout);
#endif // _DEBUG
	WinInit();
	d3dRenderer.Init();
	threadManager.Initialize();
	InitImGUI();
}

void WinGameApp::Run()
{
	if (RunApp)
	{
		__debugbreak(); //2���� �� ���� ���� �Ұ�.
		return;
	}

	using namespace TimeSystem;
	RunApp = this;
	Time.UpdateTime();
	Start();
	// PeekMessage �޼����� ������ true,������ false
	while (!isEnd)
	{	
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
	
			TranslateMessage(&msg); // Ű�Է°��� �޽��� ��ȯ  WM_KEYDOWN -> WM_CHAR
			DispatchMessage(&msg);
		}
		else
		{	
			Time.UpdateTime();
			Update();
			Render();
		}
	}
}

void WinGameApp::Uninitialize()
{
#ifdef _DEBUG
	FreeConsole();
#endif // _DEBUG
	UninitImGUI();
	d3dRenderer.Uninit();
}

void WinGameApp::WinToScreenCenter(HWND hwnd)
{
	int x, y, width, height;
	RECT rtDesk, rtWindow;
	GetWindowRect(GetDesktopWindow(), &rtDesk);
	GetWindowRect(hwnd, &rtWindow);
	width = rtWindow.right - rtWindow.left;
	height = rtWindow.bottom - rtWindow.top;
	x = (rtDesk.right - width) / 2;
	y = (rtDesk.bottom - height) / 2;
	MoveWindow(hwnd, x, y, width, height, TRUE);
}

void WinGameApp::GameEnd()
{
	if (RunApp)
	{
		RunApp->isEnd = true;
		RunApp = nullptr;
	}
}

int WinGameApp::GetFPS()
{
	if (RunApp)
	{
		TimeSystem::Time.GetFrameRate();
	}
	return -1;
}

bool WinGameApp::WinInit()
{
	//static hInstance
	if (WinGameApp::hInstance == NULL)
	{
		WinGameApp::hInstance = hInstance;
	}																  

	// ������ Ŭ���� ����ü �ʱ�ȭ
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = winClassStyle;
	if (customWndProc != nullptr)
	{	
		wc.lpfnWndProc = customWndProc;
	}
	else
	{
		wc.lpfnWndProc = DefaultWndProc;	// �⺻ ������ ���ν��� �Լ�
	}	
	wc.hInstance = hInstance;   // �ν��Ͻ� �ڵ�
	// NULL�� ����ϸ� ���� ���� ���� ���α׷��� �ν��Ͻ� �ڵ��� ����ϰ� �˴ϴ�.�� ��° ���ڴ� �ε��� Ŀ���� ID�Դϴ�.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = winClassName;

	// ������ Ŭ���� ���
	if (!RegisterClassEx(&wc))
	{
		int error = GetLastError();
		wchar_t errorStr[25]{};
		swprintf_s(errorStr, L"������ Ŭ���� ��� ����. ���� : %d", error);

		MessageBox(NULL, errorStr, L"����", MB_OK | MB_ICONERROR);
		isEnd = false;
		return false;
	}

	// ���ϴ� ũ�Ⱑ �����Ǿ� ����
	WinGameApp::size = clientSize;
	SIZE clientSize = WinGameApp::size;
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, windowStyle, FALSE);

	// ������ ����
	WinGameApp::hwnd = CreateWindowEx(
		0,
		winClassName,
		windowName,
		windowStyle,
		0, 0,
		clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
		NULL, NULL, hInstance, NULL
	);

	if (!hwnd)
	{
		int error = GetLastError();
		wchar_t errorStr[25]{};

		swprintf_s(errorStr, L"������ ���� ����. ���� : %d", error);
		MessageBox(NULL, errorStr, L"����", MB_OK | MB_ICONERROR);
		isEnd = false;
		return false;
	}

	// ������ ǥ��
	ShowWindow(hwnd, 10);
	UpdateWindow(hwnd);
	WinToScreenCenter(hwnd);

#ifdef _DEBUG
		RECT rc;
		GetClientRect(hwnd, &rc);
		int sx = rc.right - rc.left;
		int sy = rc.bottom - rc.top;
		printf("ClientSize : %d, %d\n", sx, sy);
#endif // _DEBUG

	return true;
}

void WinGameApp::InitImGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(GetHWND());
	ImGui_ImplDX11_Init(d3dRenderer.GetDevice(), d3dRenderer.GetDeviceContext());
}

void WinGameApp::UninitImGUI()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	ImGui::ClearTempMap();
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}