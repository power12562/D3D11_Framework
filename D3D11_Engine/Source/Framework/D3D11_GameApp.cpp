#include "D3D11_GameApp.h"
#include <Framework\SceneManager.h>
#include <Framework\TimeSystem.h>
#include <Framework/D3DRenderer.h>
#include <GameObject\Base\CameraObject.h>
#include <Framework\DXTKInputSystem.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK ImGUIWndProcDefault(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
#pragma region �ݵ�� ����
	case WM_DESTROY:
		PostQuitMessage(0);
		sceneManager.currScene.reset();
		WinGameApp::GameEnd();
		break;
#pragma endregion
#pragma region DXTKInputSystem ���� ����
	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;
#pragma endregion 
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

D3D11_GameApp::D3D11_GameApp()
{
	this->customWndProc = ImGUIWndProcDefault;
}

D3D11_GameApp::~D3D11_GameApp()
{

}

void D3D11_GameApp::Start()
{
	DXTKinputSystem.Initialize(GetHWND());

	if (sceneManager.nextScene == nullptr)
		sceneManager.LoadScene<Scene>(); //�� �� �ε�

	sceneManager.NextSccene();
	sceneManager.AddObjects();
}

void D3D11_GameApp::Update()
{
	fixedElapsedTime += TimeSystem::Time.GetDeltaTime(false);
	while (fixedElapsedTime >= TimeSystem::FixedTimeStep)
	{		
		sceneManager.FixedUpdateScene();
		fixedElapsedTime -= TimeSystem::FixedTimeStep;
	}
	sceneManager.UpdateScene();
	sceneManager.LateUpdateScene();
	sceneManager.UpdateTransformScene();
}

void D3D11_GameApp::Render()
{
	sceneManager.RenderScene(); //�� ������ �Լ�.
	sceneManager.AddObjects();  //������Ʈ ����
	sceneManager.EraseObjects(); //������Ʈ ����.
	sceneManager.NextSccene(); //���� �� ������ ��ȯ 
}
