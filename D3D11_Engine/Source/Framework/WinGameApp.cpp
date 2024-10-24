#include "WinGameApp.h"

#include <Framework\D3DRenderer.h>
#include <Framework/TimeSystem.h>
#include <cassert>
#include <cstdio>
#include <clocale>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>


LRESULT CALLBACK DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void WinGameApp::Initialize(HINSTANCE hinstance)
{
	if (RunApp != nullptr)
	{
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
	InitImGUI();
}

void WinGameApp::Run()
{
	using namespace TimeSystem;
	RunApp = this;
	Time.UpdateTime();
	Start();
	// PeekMessage 메세지가 있으면 true,없으면 false
	while (!isEnd)
	{	
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
	
			TranslateMessage(&msg); // 키입력관련 메시지 변환  WM_KEYDOWN -> WM_CHAR
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
	}
}

bool WinGameApp::WinInit()
{
	//static hInstance
	if (WinGameApp::hInstance == NULL)
	{
		WinGameApp::hInstance = hInstance;
	}																  

	// 윈도우 클래스 구조체 초기화
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = winClassStyle;
	if (customWndProc != nullptr)
	{	
		wc.lpfnWndProc = customWndProc;
	}
	else
	{
		wc.lpfnWndProc = DefaultWndProc;	// 기본 윈도우 프로시저 함수
	}	
	wc.hInstance = hInstance;   // 인스턴스 핸들
	// NULL을 사용하면 현재 실행 중인 프로그램의 인스턴스 핸들을 사용하게 됩니다.두 번째 인자는 로드할 커서의 ID입니다.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = winClassName;

	// 윈도우 클래스 등록
	if (!RegisterClassEx(&wc))
	{
		int error = GetLastError();
		wchar_t errorStr[25]{};
		swprintf_s(errorStr, L"윈도우 클래스 등록 실패. 에러 : %d", error);

		MessageBox(NULL, errorStr, L"에러", MB_OK | MB_ICONERROR);
		isEnd = false;
		return false;
	}

	// 원하는 크기가 조정되어 리턴
	WinGameApp::size = clientSize;
	SIZE clientSize = WinGameApp::size;
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, windowStyle, FALSE);

	// 윈도우 생성
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

		swprintf_s(errorStr, L"윈도우 생성 실패. 에러 : %d", error);
		MessageBox(NULL, errorStr, L"에러", MB_OK | MB_ICONERROR);
		isEnd = false;
		return false;
	}

	// 윈도우 표시
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
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
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