#pragma once
#include <Framework\WinGameApp.h>

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
	void FixedUpdate();

};