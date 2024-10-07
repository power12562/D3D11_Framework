#pragma once
#include <Component\Base\Component.h>
#include <Framework\DXTKInputSystem.h>

class CameraMoveHelper : public Component, public InputProcesser 
{
public:
	CameraMoveHelper();
	virtual ~CameraMoveHelper() override = default;

public:
	virtual void Start()		override;
protected:
	virtual void FixedUpdate()  override;
	virtual void Update()		override;
	virtual void LateUpdate()	override;

public:
	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyState, const DirectX::Keyboard::KeyboardStateTracker& KeyTracker,
		const DirectX::Mouse::State& MouseState, const DirectX::Mouse::ButtonStateTracker& MouseTracker) override;

public:
	void Reset();

private:
	Vector3 inputVector{};
	float moveSpeed;
	float rotSpeed;
};