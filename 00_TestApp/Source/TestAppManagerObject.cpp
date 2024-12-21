#include "TestAppManagerObject.h"
#include "../../12_Shadow/Source/ShadowTestScene.h"
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

void TestAppManagerObject::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, 
	const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
    if (KeyState.IsKeyDown(Keyboard::F5))
    {
        if (mainScene)
            sceneManager.LoadScene<ShadowTestScene>();
        else
            sceneManager.LoadScene<TestMainScene>();

        mainScene = !mainScene;
    }

    if (KeyState.IsKeyDown(Keyboard::Escape))
    {
        D3D11_GameApp::GameEnd();
    }
}
