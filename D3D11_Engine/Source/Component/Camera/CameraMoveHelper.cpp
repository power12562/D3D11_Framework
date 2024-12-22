#include "CameraMoveHelper.h"
#include <Core/TimeSystem.h>
#include <Math\Mathf.h>

CameraMoveHelper::CameraMoveHelper()
{
	rotSpeed = 0.1f;
	moveSpeed = 100.0f;
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
	if (yawRotation)
	{
		transform.rotation *= Quaternion::CreateFromAxisAngle(Vector3::UnitX, -yawRotation);
		yawRotation = 0;
	}
	if (pitchRotation)
	{
		transform.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY, -pitchRotation) * transform.rotation;
		pitchRotation = 0;
	}
	inputVector = Vector3::Zero;
}

void CameraMoveHelper::LateUpdate()
{

}

void CameraMoveHelper::OnInputProcess(const DirectX::Keyboard::State& KeyState, const DirectX::Keyboard::KeyboardStateTracker& KeyTracker, const DirectX::Mouse::State& MouseState, const DirectX::Mouse::ButtonStateTracker& MouseTracker)
{
	if (MouseTracker.rightButton == Mouse::ButtonStateTracker::ButtonState::HELD)
	{
		Vector3 forward = transform.Front;
		Vector3 right = transform.Right;

		if (KeyTracker.IsKeyPressed(Keyboard::Keys::R))
		{
			Reset();
		}

		if (KeyState.IsKeyDown(Keyboard::Keys::W))
		{
			inputVector += forward;
		}
		else if (KeyState.IsKeyDown(Keyboard::Keys::S))
		{
			inputVector += -forward;
		}

		if (KeyState.IsKeyDown(Keyboard::Keys::A))
		{
			inputVector += -right;
		}
		else if (KeyState.IsKeyDown(Keyboard::Keys::D))
		{
			inputVector += right;
		}

		if (KeyState.IsKeyDown(Keyboard::Keys::E))
		{
			inputVector += -Vector3::Up;
		}
		else if (KeyState.IsKeyDown(Keyboard::Keys::Q))
		{
			inputVector += Vector3::Up;
		}

		if (KeyState.IsKeyDown(Keyboard::Keys::F1))
		{
			moveSpeed = 10;
		}
		else if (KeyState.IsKeyDown(Keyboard::Keys::F2))
		{
			moveSpeed = 100;
		}
		else if (KeyState.IsKeyDown(Keyboard::Keys::F3))
		{
			moveSpeed = 1000;
		}

		DXTKinputSystem.GetMouse().SetMode(Mouse::MODE_RELATIVE);
		if(MouseState.positionMode == Mouse::MODE_RELATIVE)
		{
			Vector2 delta = Vector2(float(MouseState.x), float(MouseState.y)) * rotSpeed * Mathf::Deg2Rad;
			AddPitch(delta.x);
			AddYaw(delta.y);
		}
	}
	else
	{
		DXTKinputSystem.GetMouse().SetMode(Mouse::MODE_ABSOLUTE);
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
	yawRotation = value;
}

void CameraMoveHelper::AddPitch(float value)
{
	pitchRotation = value;
}
