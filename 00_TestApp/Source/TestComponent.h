#pragma once
#include <Component/Base/Component.h>

class TestComponent : public Component
{
public:
	TestComponent();
	virtual ~TestComponent() override;

	virtual void Start() override; //초기화 함수는 명시적으로 호출 가능.
protected:
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate()override;

private:
	float elapsedTime = 0;
};