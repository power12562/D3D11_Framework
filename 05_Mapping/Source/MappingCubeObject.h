#pragma once
#include <Component\Base\RenderComponent.h>

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
	DRAW_INDEX_DATA drawData;

private:
	ID3D11ShaderResourceView* m_pTextureRV = nullptr;	
	ID3D11ShaderResourceView* m_pNormalMap = nullptr;	
	ID3D11ShaderResourceView* m_pSpecularMap = nullptr;	

	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// 샘플러 상태.
};