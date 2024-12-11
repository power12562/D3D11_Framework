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
	void SetDefaultTransform(Transform& defaultTransform);
	void Reset();
	void AddPitch(float angle);
	void AddYaw(float angle);

public:
	float moveSpeed;
	float rotSpeed;

private:
	Transform startTransform;

	Vector3 inputVector{};
	Quaternion angle{}; 
};