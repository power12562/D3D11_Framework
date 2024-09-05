#pragma once
#include <Windows.h>

class WinGameApp
{
	//실행중인 앱
	inline static WinGameApp* RunApp{};
public:
	WinGameApp() = default;
	virtual ~WinGameApp() = default;

	void Initialize(HINSTANCE hinstance);
	void Run();
	void Uninitialize();

public:
	/** 현재 실행중인 게임루프를 종료시킵니다.*/
	static void GameEnd();

protected:
	/** 클래스 스타일. Initialize() 실행전에만 적용됩니다.*/
	UINT winClassStyle = CS_HREDRAW | CS_VREDRAW;

	/** 클라이언트 윈도우 스타일. Initialize() 실행전에만 적용됩니다.*/
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;

	/** 윈도우 클래스 구조체 이름. Initialize() 실행전에만 적용됩니다.*/
	LPCWSTR winClassName = L"GAME_ENGINE";

	/** 윈도우 클라이언트 이름. Initialize() 실행전에만 적용됩니다.*/
	LPCWSTR windowName = L"DemoApp";

	/** 사용할 윈도우 프로시져 (nullptr이면 기본 프로시져를 사용합니다.) Initialize() 실행전에만 적용됩니다.*/
	WNDPROC customWndProc = nullptr;

protected:
	virtual void Update() = 0;
	virtual void Render() = 0;

private:
	bool isEnd = false;
	MSG msg;
private:
	void WinInitialize();

private:
	/** 클라이언트 사이즈. 기본 값 : 1920, 1080*/
	inline static SIZE size{};
	inline static HINSTANCE hInstance;
	inline static HWND hwnd;

	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};