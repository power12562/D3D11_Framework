#pragma once
#include <Component/Base/Component.h>

class TestCubeComponent : public Component
{
public:
	TestCubeComponent();
	virtual ~TestCubeComponent() override;

public:
	virtual void Start();

protected:
	virtual void FixedUpdate();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();

private:
	struct ID3D11Buffer* m_pVertexBuffer = nullptr;				// ���ؽ� ����.
	unsigned int m_VertexBufferStride = 0;					// ���ؽ� �ϳ��� ũ��.
	unsigned int m_VertexBufferOffset = 0;					// ���ؽ� ������ ������.
	struct ID3D11Buffer* m_pIndexBuffer = nullptr;					// ���ؽ� ����.
	int m_nIndices = 0;										// �ε��� ����.

	struct ID3D11VertexShader* m_pVertexShader = nullptr;	// ���� ���̴�.
	struct ID3D11PixelShader*  m_pPixelShader = nullptr;	// �ȼ� ���̴�.	
	struct ID3D11InputLayout*  m_pInputLayout = nullptr;	// �Է� ���̾ƿ�.
};