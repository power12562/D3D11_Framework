#include "D3D11_GameApp.h"
#include <Manager/SceneManager.h>
#include <Core/TimeSystem.h>
#include <D3DCore/D3DRenderer.h>
#include <GameObject\Base\CameraObject.h>
#include <Core/DXTKInputSystem.h>
#include <Utility/ImguiHelper.h>
#include <Manager/ResourceManager.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static void ClearGame()
{
	WinGameApp::GameEnd();
}

LRESULT CALLBACK ImGUIWndProcDefault(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
#pragma region DXTKInputSystem 사용시 포함
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

	case WM_SYSKEYDOWN:
	{
		if (wParam == VK_RETURN) // Alt + Enter 입력시
		{
			d3dRenderer.ToggleFullscreenMode();
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	case WM_STYLECHANGED:
	{
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		sceneManager.currScene.reset();
		ClearGame();
		break;
	case WM_EXITSIZEMOVE: 
	{
		break;
	}

	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE) 
		{
			if (!d3dRenderer.IsSwapChainWindowed() && !sceneManager.EndGame)
			{
				d3dRenderer.ToggleFullscreenMode();
			}
		}
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			sceneManager.EndGame = true;
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void D3D11_GameApp::GameEnd()
{
	sceneManager.EndGame = true;
}

void D3D11_GameApp::ChangeResolution(SIZE resize)
{
	if (D3D11_GameApp* app = static_cast<D3D11_GameApp*>(RunApp))
	{
		D3D11_GameApp::ClampScreenMaxSize(resize);
		WinClientResize(GetHWND(), (int)resize.cx, (int)resize.cy);
		DXGI_SWAP_CHAIN_DESC1 desc{};
		Utility::CheckHRESULT(d3dRenderer.GetSwapChain()->GetDesc1(&desc));
		desc.Width = app->clientSize.cx;
		desc.Height = app->clientSize.cy;
		d3dRenderer.ReCreateSwapChain(&desc);
	}
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
	gameObjectFactory.Initialize();
	DXTKinputSystem.Initialize(GetHWND());

	if (sceneManager.nextScene == nullptr)
		sceneManager.LoadScene<Scene>(); //빈 씬 로드

	sceneManager.ChangeScene();
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
}

void D3D11_GameApp::Render()
{
	sceneManager.RenderScene(); //씬 렌더링 함수.
	sceneManager.AddObjects();  //오브젝트 생성
	sceneManager.EraseObjects(); //오브젝트 삭제.
	sceneManager.ChangeScene(); //다음 씬 있으면 전환 
}


void D3D11_GameApp::SetBorderlessWindowed()
{
	windowStyleEX = WS_POPUP;
}

void D3D11_GameApp::SetOptimalScreenSize()
{
	clientSize = { 0, 0 };
}