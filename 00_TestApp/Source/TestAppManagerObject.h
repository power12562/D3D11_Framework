#pragma once
#include <framework.h>

class TestAppManagerObject : public GameObject, public InputProcesser
{
	SERIALIZED_OBJECT(TestAppManagerObject)
public:
	TestAppManagerObject();
	virtual ~TestAppManagerObject() override;

protected:
	virtual void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker,
		const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) override;

private:
	bool mainScene = true;
};