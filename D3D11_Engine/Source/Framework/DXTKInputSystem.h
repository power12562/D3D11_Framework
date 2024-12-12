#pragma once
#include <Windows.h>
#include <Framework\TSingleton.h>
#include <directXTK/Mouse.h>
#include <directXTK/Keyboard.h>
#include <vector>
#include <windef.h>

using namespace DirectX;
class DXTKInputSystem;
extern DXTKInputSystem& DXTKinputSystem;

class InputProcesser
{
	friend class DXTKInputSystem;
protected:
	InputProcesser();
	~InputProcesser();

protected:
	virtual void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker,
		const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) = 0;
};

class DXTKInputSystem : public TSingleton<DXTKInputSystem>
{
	friend class TSingleton;
	friend InputProcesser::InputProcesser();
	friend InputProcesser::~InputProcesser();		
private:
	DXTKInputSystem() = default;
	~DXTKInputSystem() = default;

	std::vector<InputProcesser*>					inputProcessersList;

	std::unique_ptr<DirectX::Keyboard>              keyboard;
	std::unique_ptr<DirectX::Mouse>                 mouse;
	DirectX::Keyboard::KeyboardStateTracker         keyboardStateTracker;
	DirectX::Mouse::ButtonStateTracker              mouseStateTracker;

	DirectX::Mouse::State                           mouseState{};
	DirectX::Keyboard::State                        keyboardState{};

public:
	void Update();
	void Initialize(HWND hWnd);

	DirectX::Mouse& GetMouse(){ return *mouse; }

};