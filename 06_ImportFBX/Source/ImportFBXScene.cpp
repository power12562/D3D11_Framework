#include "ImportFBXScene.h"
#include <Framework\SceneManager.h>
#include <Component\CameraMoveHelper.h>

#include "../Source/AssimpComponent.h"
#include "../Source/SimpleDirectionalLight.h"

ImportFBXScene::ImportFBXScene()
{
	UseImGUI = true;
	auto cam = NewGameObject<GameObject>(L"MainCamera");
	cam->AddComponent<CameraMoveHelper>();
	cam->AddComponent<SimpleDirectionalLight>();

	auto obj1 = NewGameObject<GameObject>(L"obj1");
	obj1->AddComponent<AssimpComponent>();
}

ImportFBXScene::~ImportFBXScene()
{
}

void ImportFBXScene::ImGUIRender()
{
}
