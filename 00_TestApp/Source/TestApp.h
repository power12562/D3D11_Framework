#pragma once
#include <D3DCore/D3D11_GameApp.h>
#include <vector>

class TestApp : public D3D11_GameApp
{
public:
	TestApp();
	~TestApp();

private:
	/*마우스 메시지 처리 전 호출해주는 메서드*/
	virtual void PreProcessMouseMessage(UINT message, MouseVK vk) override;
	/*키보드 메시지 처리 전 호출해주는 메서드*/
	virtual void PreProcessKeyboradMessage(UINT message, KeyboardVK vk) override;
};