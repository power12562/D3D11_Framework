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
	ID3D11Buffer* pVertexBuffer = nullptr;				    // ���ؽ� ����.
	unsigned int vertexBufferStride = 0;					// ���ؽ� �ϳ��� ũ��.
	unsigned int vertexBufferOffset = 0;					// ���ؽ� ������ ������.

	ID3D11Buffer* pIndexBuffer = nullptr;					// �ε��� ����.
	int indicesCount = 0;									// �ε��� ����.

private:
	SimpleMaterial material;
};