#include "SceneManager.h"
#include <GameObject\Base\CameraObject.h>

SceneManager& sceneManager = SceneManager::GetInstance();

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::CheckMainCam()
{
	if (Camera::GetMainCamera() == nullptr)
	{
		auto mainCamera = new CameraObject(L"MainCamera");
		mainCamera->SetMainCamera();
		nextScene->objectList.emplace_back(mainCamera);
		nextAddQueue.pop();
	}
}

void SceneManager::FixedUpdateScene()
{
	currScene->FixedUpdate();
}

void SceneManager::UpdateScene()
{
	currScene->Update();
}

void SceneManager::LateUpdateScene()
{
	currScene->LateUpdate();
}

void SceneManager::UpdateTransformScene()
{
	currScene->UpdateTransform();
}

void SceneManager::RenderScene()
{
	currScene->Render();
}

void SceneManager::NextSccene()
{
	if (nextScene)
	{
		currScene = std::move(nextScene);
		nextScene = nullptr;
	}
}

void SceneManager::AddObjects()
{
	while (!currAddQueue.empty())
	{
		GameObject* obj = currAddQueue.front();
		currScene->objectList.emplace_back(obj);
		currAddQueue.pop();
	}
}
						

