#pragma once
#include <Framework\WinGameApp.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

class D3D11_GameApp : public WinGameApp
{
public:
	D3D11_GameApp();
	virtual ~D3D11_GameApp() override;

private:
	virtual void Start() override final;
	virtual void Update() override final;
	virtual void Render() override final;

private:
	float fixedElapsedTime = 0;
};