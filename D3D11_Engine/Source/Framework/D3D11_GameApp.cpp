#include "D3D11_GameApp.h"
#include <Framework\SceneManager.h>
#include <Framework\TimeSystem.h>
#include <GameObject\Base\CameraObject.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

D3D11_GameApp::D3D11_GameApp()
{

}

D3D11_GameApp::~D3D11_GameApp()
{

}

void D3D11_GameApp::Start()
{
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
	sceneManager.NextSccene(); //다음 씬 있으면 전환 
}
