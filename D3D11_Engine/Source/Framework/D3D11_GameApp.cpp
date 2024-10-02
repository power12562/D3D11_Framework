#include "D3D11_GameApp.h"
#include <Framework\SceneManager.h>
#include <Framework\TimeSystem.h>

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
	sceneManager.LoadScene<Scene>(); //ºó ¾À ·Îµå
	sceneManager.NextSccene();
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

}
