#pragma once
#include <D3DCore/D3D11_GameApp.h>
#include <vector>

class TestApp : public D3D11_GameApp
{
public:
	TestApp();
	~TestApp();

private:
	/*���콺 �޽��� ó�� �� ȣ�����ִ� �޼���*/
	virtual void PreProcessMouseMessage(UINT message, MouseVK vk) override;
	/*Ű���� �޽��� ó�� �� ȣ�����ִ� �޼���*/
	virtual void PreProcessKeyboradMessage(UINT message, KeyboardVK vk) override;
};