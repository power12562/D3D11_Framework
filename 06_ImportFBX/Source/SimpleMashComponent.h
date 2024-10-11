#pragma once
#include <Component/Base/RenderComponent.h>

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector3 biTangent;
	Vector2 Tex;
};

class SimpleMashComponent : public RenderComponent
{
public:
	SimpleMashComponent();
	virtual ~SimpleMashComponent() override;

public:
	void SetMesh();

public:
	virtual void Start()		override; //�ʱ�ȭ �Լ��� ��������� ȣ�� ����.

protected:
	virtual void FixedUpdate()	override;
	virtual void Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render()		override;

public:
	std::vector<Vertex> vertices;	//���ؽ� ����
	std::vector<UINT> indices;		//�ε��� ����
	DRAW_INDEX_DATA drawData;	   

	ID3D11ShaderResourceView* m_pTextureRV = nullptr;
	ID3D11ShaderResourceView* m_pNormalMap = nullptr;
	ID3D11ShaderResourceView* m_pSpecularMap = nullptr;
private:

	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// ���÷� ����.
};