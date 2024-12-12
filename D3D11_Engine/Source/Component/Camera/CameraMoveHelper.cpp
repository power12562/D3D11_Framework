#include "CameraMoveHelper.h"
#include <Framework\TimeSystem.h>
#include <Math\Mathf.h>

CameraMoveHelper::CameraMoveHelper()
{
	rotSpeed = 15;
	moveSpeed = 10.0f;
}

void CameraMoveHelper::Start()
{
	SetDefaultTransform(transform);
}

void CameraMoveHelper::FixedUpdate()
{

}

void CameraMoveHelper::Update()
{
	inputVector.Normalize();
	if (inputVector.Length() > 0.0f)
	{
		transform.position += inputVector * moveSpeed * TimeSystem::Time.DeltaTime;
		inputVector = Vector3::Zero;
	}
	if (yawRotation || pitchRotation)
	{
		transform.rotation *= Quaternion::CreateFromAxisAngle(Vector3::UnitX, -yawRotation * TimeSystem::Time.DeltaTime);
		transform.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY, -pitchRotation * TimeSystem::Time.DeltaTime) * transform.rotation;
		yawRotation = 0;
		pitchRotation = 0;
	}
	inputVector = Vector3::Zero;
}

void CameraMoveHelper::LateUpdate()
{

}

void CameraMoveHelper::OnInputProcess(const DirectX::Keyboard::State& KeyState, const DirectX::Keyboard::KeyboardStateTracker& KeyTracker, const DirectX::Mouse::State& MouseState, const DirectX::Mouse::ButtonStateTracker& MouseTracker)
{
	Vector3 forward = transform.Front;
	Vector3 right = transform.Right;

	if (KeyTracker.IsKeyPressed(Keyboard::Keys::R))
	{
		Reset();
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::W))
	{
		inputVector += forward;
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::S))
	{
		inputVector += -forward;
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::A))
	{
		inputVector += -right;
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::D))
	{
		inputVector += right;
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::E))
	{
		inputVector += -Vector3::Up;
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::Q))
	{
		inputVector += Vector3::Up;
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::F1))
	{
		moveSpeed = 10;
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::F2))
	{
		moveSpeed = 100;
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::F3))
	{
		moveSpeed = 1000;
	}

	DXTKinputSystem.GetMouse().SetMode(MouseState.rightButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);
	if (MouseState.positionMode == Mouse::MODE_RELATIVE)
	{
		Vector2 delta = Vector2(float(MouseState.x), float(MouseState.y)) * rotSpeed * Mathf::Deg2Rad;
		AddPitch(delta.x);
		AddYaw(delta.y);
	}
}

void CameraMoveHelper::SetDefaultTransform(Transform& defaultTransform)
{
	startTransform = defaultTransform;
}

void CameraMoveHelper::Reset()
{
	transform = startTransform;
}

void CameraMoveHelper::AddYaw(float value)
{	
	yawRotation += value;
}

void CameraMoveHelper::AddPitch(float value)
{
	pitchRotation += value;
}
