#include "D3D11_GameApp.h"
#include <Framework\SceneManager.h>
#include <Framework\TimeSystem.h>
#include <Framework/D3DRenderer.h>
#include <GameObject\Base\CameraObject.h>
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
	case WM_DESTROY:
		PostQuitMessage(0);
		sceneManager.currScene.reset();
		WinGameApp::GameEnd();
		break;
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
	if (sceneManager.nextScene == nullptr)
		sceneManager.LoadScene<Scene>(); //빈 씬 로드

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
	sceneManager.RenderScene(); //씬 렌더링 함수.
	sceneManager.AddObjects();  //오브젝트 생성
	sceneManager.EraseObjects(); //오브젝트 삭제.
	sceneManager.NextSccene(); //다음 씬 있으면 전환 
}
