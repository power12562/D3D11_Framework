#pragma once
#include <Component\Base\RenderComponent.h>
#include <Material\SimpleMaterial.h>

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

private:
	SimpleMaterial material;
};