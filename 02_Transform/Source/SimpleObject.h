#pragma once
#include <d3d11.h>
#include <Framework\Transform.h>
using namespace DirectX;

struct Vertex
{
	Vector3 position;		// ���� ��ġ ����.
	Vector4 color;			// ���� ���� ����.

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

	// ������ ���������ο� �����ϴ�  ��ü�� ����
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// ���ؽ� ����.
	unsigned int m_VertexBufferStride = 0;					// ���ؽ� �ϳ��� ũ��.
	unsigned int m_VertexBufferOffset = 0;					// ���ؽ� ������ ������.
	ID3D11Buffer* m_pIndexBuffer = nullptr;	// ���ؽ� ����.
	int m_nIndices = 0;								// �ε��� ����.

	// ���̴��� �� ������ ������
	Matrix m_World = XMMatrixIdentity();				// ������ǥ�� �������� ��ȯ�� ���� ���.
};