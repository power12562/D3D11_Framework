#pragma once
#include <d3d11.h>
#include <Framework\Transform.h>
using namespace DirectX;

struct Vertex
{
	Vector3 position;		// 정점 위치 정보.
	Vector4 color;			// 정점 색상 정보.

	Vertex(float x, float y, float z) : position(x, y, z) { }
	Vertex(Vector3 position) : position(position) { }

	Vertex(Vector3 position, Vector4 color)
		: position(position), color(color) { }
};

struct ConstantBuffer
{
	Matrix mWorld;
	Matrix mView;
	Matrix mProjection;
	Matrix WVP;
};
static_assert((sizeof(ConstantBuffer) % 16) == 0, "Constant Buffer size must be 16 - byte aligned");

class Transform;
class SimpleObject
{
public:
	SimpleObject();
	virtual ~SimpleObject();

	virtual void Update() = 0;
	virtual void Render(struct ID3D11Buffer* pConstantBuffer, struct ID3D11InputLayout* m_pInputLayout, struct ID3D11VertexShader* pVertexShader, struct ID3D11PixelShader* pPixelShader) = 0;
public:
	Transform* transform;  
public:
	inline static Matrix mView = XMMatrixIdentity();
	inline static Matrix mProjection = XMMatrixIdentity();
};

class CubeObject : public SimpleObject
{
public:
	CubeObject();
	virtual ~CubeObject() override;

	virtual void Update() override;
	virtual void Render(ID3D11Buffer* pConstantBuffer, ID3D11InputLayout* pInputLayout, ID3D11VertexShader* pVertexShader, ID3D11PixelShader* pPixelShader) override;

	// 렌더링 파이프라인에 적용하는  객체와 정보
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// 버텍스 버퍼.
	unsigned int m_VertexBufferStride = 0;					// 버텍스 하나의 크기.
	unsigned int m_VertexBufferOffset = 0;					// 버텍스 버퍼의 오프셋.
	ID3D11Buffer* m_pIndexBuffer = nullptr;	// 버텍스 버퍼.
	int m_nIndices = 0;								// 인덱스 개수.

	// 쉐이더에 에 전달할 데이터
	Matrix m_World = XMMatrixIdentity();				// 월드좌표계 공간으로 변환을 위한 행렬.
};