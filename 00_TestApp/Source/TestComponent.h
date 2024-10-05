#pragma once
#include <Component/Base/Component.h>

class TestComponent : public Component
{
public:
	TestComponent();
	virtual ~TestComponent() override;

	virtual void Start() override; //�ʱ�ȭ �Լ��� ��������� ȣ�� ����.
protected:
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate()override;

private:
	float elapsedTime = 0;
};