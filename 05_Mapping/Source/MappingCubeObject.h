#pragma once
#include <Component\Base\RenderComponent.h>
#include <Material\BlingPhongMaterial.h>
#include <D3DCore/D3DConstBuffer.h>
#include <D3DCore/D3DSamplerState.h>
#include <D3DCore/D3DTexture2D.h>

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
	ID3D11InputLayout*  pInputLayout = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader*  pPixelShader = nullptr;

private:
	D3DSamplerState sampler;
	D3DTexture2D textures;
	DRAW_INDEX_DATA dd;
};