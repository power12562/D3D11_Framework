#pragma once
#include <Component\Base\RenderComponent.h>
#include <Material\BlingPhongMaterial.h>
#include <Framework/D3DConstBuffer.h>
#include <Framework/D3DSamplerState.h>
#include <Framework/D3DTexture2D.h>

class MappingCubeObject	: public RenderComponent
{
public:
	MappingCubeObject();
	virtual ~MappingCubeObject() override;

public:
	virtual void Start() override;
protected:
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	ID3D11Buffer* pVertexBuffer = nullptr;				    // ���ؽ� ����.
	unsigned int vertexBufferStride = 0;					// ���ؽ� �ϳ��� ũ��.
	unsigned int vertexBufferOffset = 0;					// ���ؽ� ������ ������.

	ID3D11Buffer* pIndexBuffer = nullptr;					// �ε��� ����.
	int indicesCount = 0;									// �ε��� ����.

	ID3D11InputLayout*  pInputLayout = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader*  pPixelShader = nullptr;

private:
	D3DSamplerState sampler;
	D3DTexture2D texture2D;
};