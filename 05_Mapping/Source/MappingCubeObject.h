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
	ID3D11Buffer* pVertexBuffer = nullptr;				    // 버텍스 버퍼.
	unsigned int vertexBufferStride = 0;					// 버텍스 하나의 크기.
	unsigned int vertexBufferOffset = 0;					// 버텍스 버퍼의 오프셋.

	ID3D11Buffer* pIndexBuffer = nullptr;					// 인덱스 버퍼.
	int indicesCount = 0;									// 인덱스 개수.

	ID3D11InputLayout*  pInputLayout = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader*  pPixelShader = nullptr;

private:
	D3DSamplerState sampler;
	D3DTexture2D texture2D;
};