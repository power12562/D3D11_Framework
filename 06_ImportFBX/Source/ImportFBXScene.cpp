#include "ImportFBXScene.h"
#include <Framework\SceneManager.h>
#include <Component\CameraMoveHelper.h>
#include <Framework/D3DRenderer.h>
#include <GameObject/Base/CameraObject.h>

#include "../Source/AssimpComponent.h"
#include "../Source/SimpleDirectionalLight.h"

ImportFBXScene::ImportFBXScene()
{
	UseImGUI = true;
	d3dRenderer.backgroundColor = Color(1, 1, 1, 1);
	auto cam = NewGameObject<CameraObject>(L"MainCamera");
	cam->SetMainCamera();
	cam->AddComponent<CameraMoveHelper>();
	cam->AddComponent<SimpleDirectionalLight>();
	cam->transform.position = Vector3(0, 0, -10);

	auto objBase = NewGameObject<GameObject>(L"objBase");
	objBase->transform.scale = { 0.1, 0.1, 0.1 };
	auto obj = NewGameObject<GameObject>(L"obj");
	obj->AddComponent<AssimpComponent>();
	obj->transform.SetParent(objBase->transform, false);
}

ImportFBXScene::~ImportFBXScene()
{
}

void ImportFBXScene::ImGUIRender()
{
}
