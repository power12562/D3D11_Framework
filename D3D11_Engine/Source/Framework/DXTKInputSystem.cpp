#include "DXTKInputSystem.h"
#include <Utility\D3D11Utility.h>


using namespace DirectX;
using namespace DirectX::SimpleMath;

DXTKInputSystem& DXTKinputSystem = DXTKInputSystem::GetInstance();

InputProcesser::InputProcesser()
{
	//DXTKinputSystem.inputProcessersList.push_back(dynamic_cast<InputProcesser*>(this));
	DXTKinputSystem.inputProcessersList.push_back(this);
}

InputProcesser::~InputProcesser()
{
   auto& inputList = DXTKinputSystem.inputProcessersList;
   inputList.erase(std::find(inputList.begin(), inputList.end(), this));
}


void DXTKInputSystem::Update()
{
	mouseState = mouse->GetState();
	mouseStateTracker.Update(mouseState);

	keyboardState = keyboard->GetState();
	keyboardStateTracker.Update(keyboardState);

	if (!inputProcessersList.empty())
	{
		for (auto& proc : inputProcessersList)
		{
			proc->OnInputProcess(keyboardState, keyboardStateTracker, mouseState, mouseStateTracker);
		}
	}
}

void DXTKInputSystem::Initialize(HWND hWnd)
{
	keyboard = std::make_unique<Keyboard>();
	mouse = std::make_unique<Mouse>();
	mouse->SetWindow(hWnd);
}

