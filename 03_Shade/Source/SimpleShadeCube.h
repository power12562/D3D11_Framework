#pragma once
#include <d3d11.h>
#include <Framework\Transform.h>
using namespace DirectX;

struct Vertex
{
	Vector3 position;		// 정점 위치 정보.
	Vector3 normal;			// 정점 색상 정보.
	Vector2 Tex;
};

struct ConstantBuffer
{
	Matrix mWorld;
	Matrix mView;
	Matrix mProjection;
	Matrix WVP;
	Vector4 vLightDir;
	Vector4 vLightColor;
};
static_assert((sizeof(ConstantBuffer) % 16) == 0, "Constant Buffer size must be 16 - byte aligned");

class Transform;
class SimpleShadeObject
{
public:
	SimpleShadeObject();
	virtual ~SimpleShadeObject();

	virtual void Update() = 0;
	virtual void Render(struct ID3D11Buffer* pConstantBuffer, struct ID3D11InputLayout* m_pInputLayout, struct ID3D11VertexShader* pVertexShader, struct ID3D11PixelShader* pPixelShader) = 0;
public:
	Transform* transform;  

	inline static Matrix mView{};
	inline static Matrix mProjection{};
	inline static Vector4 vLightDir{};
	inline static Vector4 vLightColor{};

};

class SimpleCubeShadeObject : public SimpleShadeObject
{
public:
	SimpleCubeShadeObject();
	virtual ~SimpleCubeShadeObject() override;

	virtual void Update() override;
	virtual void Render(ID3D11Buffer* pConstantBuffer, ID3D11InputLayout* pInputLayout, ID3D11VertexShader* pVertexShader, ID3D11PixelShader* pPixelShader) override;

	// 렌더링 파이프라인에 적용하는  객체와 정보
	ID3D11Buffer* m_pVertexBuffer = nullptr;				// 버텍스 버퍼.
	unsigned int m_VertexBufferStride = 0;					// 버텍스 하나의 크기.
	unsigned int m_VertexBufferOffset = 0;					// 버텍스 버퍼의 오프셋.
	ID3D11Buffer* m_pIndexBuffer = nullptr;					// 버텍스 버퍼.
	int m_nIndices = 0;										// 인덱스 개수.
	ID3D11ShaderResourceView* m_pTextureRV = nullptr;	// 텍스처 리소스 뷰.
	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// 샘플러 상태.
};

