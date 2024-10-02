#pragma once
#include <Framework\WinGameApp.h>

class D3D11_GameApp : public WinGameApp
{
public:
	D3D11_GameApp();
	virtual ~D3D11_GameApp() override;

protected:
	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	void FixedUpdate();

};