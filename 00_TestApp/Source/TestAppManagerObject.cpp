#include "TestAppManagerObject.h"
#include "TestMainScene.h"

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
            sceneManager.LoadScene<TestMainScene>();
        else
            sceneManager.LoadScene<TestMainScene>();

        mainScene = !mainScene;
    }
   
}
