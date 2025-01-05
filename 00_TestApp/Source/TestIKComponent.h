#pragma once
#include <Component/Base/Component.h>

class TestIKComponent : public Component
{
public:
	TestIKComponent();
	virtual ~TestIKComponent();
public:
	virtual void Start() override;
protected:
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
public:
	void SetIK(GameObject* target, int maxDepth);
private:
	Transform* endEffector = nullptr;
	int maxDepth = 0;

	std::vector<Transform*> chain;
	void CCDMode();
};