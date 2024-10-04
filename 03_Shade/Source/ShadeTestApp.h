#pragma once
#include <Framework/WinGameApp.h>

class ShadeTestApp : public WinGameApp
{
public:
	ShadeTestApp();
	~ShadeTestApp();

	void InitScene();
	void UninitScene();

protected:
	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	void ImGUIRender();

private:
	struct ID3D11VertexShader*	 m_pVertexShader = nullptr;	// ���� ���̴�.
	struct ID3D11PixelShader*	 m_pPixelShader = nullptr;	// �ȼ� ���̴�.	
	struct ID3D11InputLayout*	 m_pInputLayout = nullptr;	// �Է� ���̾ƿ�.
	struct ID3D11Buffer*		 m_pConstantBuffer = nullptr;		// ��� ����.

private:
	class SimpleCubeShadeObject* m_cubeObject = nullptr;

};

namespace constBuffer
{
	extern float lightDir[4];
	extern float lightColor[4];
	extern float camPos[3];
	extern float camFOV;
	extern float cubeRotation[3];
	extern float bgColor[4];
}