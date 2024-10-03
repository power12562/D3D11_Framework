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
	struct ID3D11Buffer* m_pVertexBuffer = nullptr;				// 버텍스 버퍼.
	unsigned int m_VertexBufferStride = 0;					// 버텍스 하나의 크기.
	unsigned int m_VertexBufferOffset = 0;					// 버텍스 버퍼의 오프셋.
	struct ID3D11Buffer* m_pIndexBuffer = nullptr;					// 버텍스 버퍼.
	int m_nIndices = 0;										// 인덱스 개수.

	struct ID3D11VertexShader* m_pVertexShader = nullptr;	// 정점 셰이더.
	struct ID3D11PixelShader*  m_pPixelShader = nullptr;	// 픽셀 셰이더.	
	struct ID3D11InputLayout*  m_pInputLayout = nullptr;	// 입력 레이아웃.
};