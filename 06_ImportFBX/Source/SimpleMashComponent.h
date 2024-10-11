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
	virtual void Start()		override; //초기화 함수는 명시적으로 호출 가능.

protected:
	virtual void FixedUpdate()	override;
	virtual void Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render()		override;

public:
	std::vector<Vertex> vertices;	//버텍스 버퍼
	std::vector<UINT> indices;		//인덱스 버퍼
	DRAW_INDEX_DATA drawData;	   

	ID3D11ShaderResourceView* m_pTextureRV = nullptr;
	ID3D11ShaderResourceView* m_pNormalMap = nullptr;
	ID3D11ShaderResourceView* m_pSpecularMap = nullptr;
private:

	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// 샘플러 상태.
};