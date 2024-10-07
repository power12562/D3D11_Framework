#include "CameraMoveHelper.h"
#include <Framework\TimeSystem.h>
#include <Math\Mathf.h>

CameraMoveHelper::CameraMoveHelper()
{
	rotSpeed = 15 * Mathf::Deg2Rad;
	moveSpeed = 20.0f;
}

void CameraMoveHelper::Start()
{
	
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
}

void CameraMoveHelper::LateUpdate()
{

}

void CameraMoveHelper::OnInputProcess(const DirectX::Keyboard::State& KeyState, const DirectX::Keyboard::KeyboardStateTracker& KeyTracker, const DirectX::Mouse::State& MouseState, const DirectX::Mouse::ButtonStateTracker& MouseTracker)
{
	Vector3 forward = transform.Front;
	Vector3 right = transform.Right;
	Vector3 up = transform.Up;

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
		inputVector += -up;
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::Q))
	{
		inputVector += up;
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::F1))
	{
		moveSpeed = 200;
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::F2))
	{
		moveSpeed = 5000;
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::F3))
	{
		moveSpeed = 10000;
	}

	DXTKinputSystem.GetMouse().SetMode(MouseState.rightButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);
	if (MouseState.positionMode == Mouse::MODE_RELATIVE)
	{
		Vector3 delta = Vector3(float(MouseState.x), float(MouseState.y), 0.f) * rotSpeed * TimeSystem::Time.DeltaTime;
		transform.rotation *= Quaternion::CreateFromYawPitchRoll(Vector3(-delta.y, -delta.x, 0));
	}
}

void CameraMoveHelper::Reset()
{
	transform.position = Vector3(0, 0, 0);
}
