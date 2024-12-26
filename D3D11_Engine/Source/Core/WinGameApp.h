#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Utility/DumpUtility.h>

#pragma comment(lib, "Dbghelp.lib")

class WinGameApp
{
protected:
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
	static const SIZE& GetClientSize() { return RunApp->clientSize; }
	static DWORD GetWindowStyleEX() { return RunApp ? RunApp->windowStyleEX : NULL; }
	//size <= 0 �̰ų� size > maxScreenSize �̸� �ִ� ũ��� �������ݴϴ�.
	static void ClampScreenMaxSize(SIZE& size);
	//size <= 0 �̰ų� size > maxScreenSize �̸� �ִ� ũ��� �������ݴϴ�.
	static void ClampClientMaxSize(SIZE& size);

	/**Ŭ���̾�Ʈ�� ȭ�� ����� ��ġ ��Ų��.*/ 
	static void WinToScreenCenter(HWND hwnd);

	/**Ŭ���̾�Ʈ�� ũ�� ����.*/
	static void WinClientResize(HWND hwnd, int width, int height);

	/** ���� �������� ���ӷ����� �����ŵ�ϴ�.*/
	static void GameEnd();

	static int GetFPS();

protected:
	/** Ŭ���� ��Ÿ��. Initialize() ���������� ����˴ϴ�.*/
	UINT winClassStyle = CS_HREDRAW | CS_VREDRAW;

	/** Ŭ���̾�Ʈ ������ ��Ÿ��. Initialize() ���������� ����˴ϴ�.*/
	DWORD windowStyleEX = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

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

protected:
	bool isEnd = false;
private:
	MSG msg{};

private:
	bool WinInit(HINSTANCE hInstance);

protected:
	virtual void InitImGUI();

private:
	void UninitImGUI();

private:
	inline static HINSTANCE hInstance{};
	inline static HWND hwnd{};

	friend LRESULT CALLBACK DefaultWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};