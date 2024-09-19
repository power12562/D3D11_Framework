#pragma once
#define WIN32_LEAN_AND_MEAN
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
	static HWND GetHWND() { return hwnd; }
	static HINSTANCE GetHINSTANCE() { return hInstance; }			
	static const SIZE& GetClientSize() { return  size; }

	/**Ŭ���̾�Ʈ�� ȭ�� ����� ��ġ ��Ų��.*/
	static void WinToScreenCenter(HWND hwnd);

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

	/** Ŭ���̾�Ʈ ������. �⺻ �� : 1920, 1080. Initialize() ���������� ����˴ϴ�.*/
	SIZE clientSize = { 1920, 1080 };

protected:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

private:
	bool isEnd = false;
	MSG msg{};
private:
	bool WinInit();

private:
	inline static SIZE size{};
	inline static HINSTANCE hInstance{};
	inline static HWND hwnd{};

	friend LRESULT CALLBACK DefaultWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};