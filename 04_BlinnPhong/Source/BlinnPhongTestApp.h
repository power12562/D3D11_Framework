#pragma once
#include <Core/WinGameApp.h>

class BlinnPhongTestApp : public WinGameApp
{
public:
	BlinnPhongTestApp();
	~BlinnPhongTestApp();

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
	class SimpleBlinnPhongObject* m_cubeObject = nullptr;

};

namespace global
{
	extern float camPos[3];
	extern float camFOV;
	extern float cubeRotation[3];
	extern float cubeScale;
	extern float bgColor[4];

	extern float LightDir[4];
	extern float LightColor[4];
	extern float LightAmbient[4];
	extern float LightSpecular[4];

	extern float MaterialAmbient[4];
	extern float MaterialDiffuse[4];
	extern float MaterialSpecular[4];
	extern float MaterialSpecularPower;
}