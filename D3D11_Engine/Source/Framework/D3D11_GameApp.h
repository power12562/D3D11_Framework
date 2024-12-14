#pragma once
#include <Framework\WinGameApp.h>

#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

class D3D11_GameApp : public WinGameApp
{
public:
	static void GameEnd();

	//�ػ� ����
	static void ChangeResolution(SIZE resize);

public:
	D3D11_GameApp();
	virtual ~D3D11_GameApp() override;

private:
	virtual void Start() override final;
	virtual void Update() override final;
	virtual void Render() override final;

protected:
	void SetBorderlessWindowed(); //��� ���� â���� ����
	void SetOptimalScreenSize(); //ȭ�� �ػ� �ڵ����� ����

private:
	float fixedElapsedTime = 0;
};