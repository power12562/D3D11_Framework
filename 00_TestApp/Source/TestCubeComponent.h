#pragma once
#include <Component/Base/RenderComponent.h>
#include <Framework\D3DRenderer.h>

struct TestCbuffer
{
	Color c{ 1,0,0,0 };
};

class TestCubeComponent : public RenderComponent
{
public:
	TestCubeComponent();
	virtual ~TestCubeComponent() override;

public:
	virtual void Start();

protected:
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	DRAW_INDEX_DATA drawData;
	float elapseTime = 0;;

private:
	TestCbuffer testColor;
};