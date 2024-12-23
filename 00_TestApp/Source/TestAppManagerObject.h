#pragma once
#include <framework.h>

class TestAppManagerObject : public GameObject, public InputProcesser
{
	SERIALIZED_OBJECT(TestAppManagerObject)
public:
	TestAppManagerObject();
	virtual ~TestAppManagerObject() override;

protected:
	virtual void OnInputProcess(DXTKInputSystem::InputSystem& Input) override;

private:
	inline static TestAppManagerObject* instance = nullptr;
	bool mainScene = true;
};