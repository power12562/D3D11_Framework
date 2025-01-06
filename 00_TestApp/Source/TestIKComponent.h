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
	int maxDepth = 0;
	float maxReach = 0.f;
	std::vector<Transform*> joints;
	void CCDMode();
	void CalculateMaxReach();
};