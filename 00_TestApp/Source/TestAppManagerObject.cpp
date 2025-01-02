#include "TestAppManagerObject.h"
#include "TestMainScene.h"
#include <framework.h>
#include "../../13_Deferred_01/Source/DeferredTestScene1.h"

TestAppManagerObject::TestAppManagerObject()
{
    if (instance)
    {
        GameObject::Destroy(this);
    }
    else
    {
        instance = this;
    }
}

TestAppManagerObject::~TestAppManagerObject()
{
}

void TestAppManagerObject::OnInputProcess(DXTKInputSystem::InputSystem& Input)
{
    if (Input.IsKeyDown(Keyboard::F5))
    {
        if (mainScene)
            sceneManager.LoadScene<DeferredTestScene1>();
        else
            sceneManager.LoadScene<TestMainScene>();

        mainScene = !mainScene;
    }

    if (Scene::GuizmoSetting.SelectObject && Input.IsKeyDown(KeyboardKeys::Enter))
    {
        if (TransformAnimation* animation = Scene::GuizmoSetting.SelectObject->IsComponent<TransformAnimation>())
        {
            animation->PlayClip(L"mixamo.com");
        }
    }
}
