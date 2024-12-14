#pragma once
#include <Framework\WinGameApp.h>

#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

class D3D11_GameApp : public WinGameApp
{
public:
	static void GameEnd();

	//해상도 변경
	static void ChangeResolution(SIZE resize);

public:
	D3D11_GameApp();
	virtual ~D3D11_GameApp() override;

private:
	virtual void Start() override final;
	virtual void Update() override final;
	virtual void Render() override final;

protected:
	void SetBorderlessWindowed(); //경계 없는 창으로 설정
	void SetOptimalScreenSize(); //화면 해상도 자동으로 설정

private:
	float fixedElapsedTime = 0;
};