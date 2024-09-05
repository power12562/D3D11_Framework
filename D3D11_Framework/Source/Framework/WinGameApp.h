#pragma once
#include <Windows.h>

class WinGameApp
{
	//�������� ��
	inline static WinGameApp* RunApp{};
public:
	WinGameApp() = default;
	virtual ~WinGameApp() = default;

	void Initialize(HINSTANCE hinstance);
	void Run();
	void Uninitialize();

public:
	/** ���� �������� ���ӷ����� �����ŵ�ϴ�.*/
	static void GameEnd();

protected:
	/** Ŭ���� ��Ÿ��. Initialize() ���������� ����˴ϴ�.*/
	UINT winClassStyle = CS_HREDRAW | CS_VREDRAW;

	/** Ŭ���̾�Ʈ ������ ��Ÿ��. Initialize() ���������� ����˴ϴ�.*/
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;

	/** ������ Ŭ���� ����ü �̸�. Initialize() ���������� ����˴ϴ�.*/
	LPCWSTR winClassName = L"GAME_ENGINE";

	/** ������ Ŭ���̾�Ʈ �̸�. Initialize() ���������� ����˴ϴ�.*/
	LPCWSTR windowName = L"DemoApp";

	/** ����� ������ ���ν��� (nullptr�̸� �⺻ ���ν����� ����մϴ�.) Initialize() ���������� ����˴ϴ�.*/
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
	/** Ŭ���̾�Ʈ ������. �⺻ �� : 1920, 1080*/
	inline static SIZE size{};
	inline static HINSTANCE hInstance;
	inline static HWND hwnd;

	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};