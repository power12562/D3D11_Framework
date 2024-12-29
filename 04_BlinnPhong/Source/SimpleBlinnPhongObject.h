#pragma once
#include <d3d11.h>
#include <Core/Transform.h>
using namespace DirectX;

struct Vertex
{
	Vector3 position;		// ���� ��ġ ����.
	Vector3 normal;			// ���� ���� ����.
	Vector2 Tex;
};

struct ConstantBuffer
{
	Matrix World;
	Matrix View;
	Matrix WV;
	Matrix WVP;

	Vector4 CamPos;

	Vector4 LightDir;
	Vector4 LightColor;
	Vector4 LightAmbient;
	Vector4 LightSpecular;

	Vector4 MaterialAmbient;
	Vector4 MaterialDiffuse;
	Vector4 MaterialSpecular;
	float MaterialSpecularPower{};
	Vector3  MaterialSpecularPad;
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

class SimpleBlinnPhongObject : public SimpleShadeObject
{
public:
	SimpleBlinnPhongObject();
	virtual ~SimpleBlinnPhongObject() override;

	virtual void Update() override;
	virtual void Render(ID3D11Buffer* pConstantBuffer, ID3D11InputLayout* pInputLayout, ID3D11VertexShader* pVertexShader, ID3D11PixelShader* pPixelShader) override;

	// ������ ���������ο� �����ϴ�  ��ü�� ����
	ID3D11Buffer* m_pVertexBuffer = nullptr;				// ���ؽ� ����.
	unsigned int m_VertexBufferStride = 0;					// ���ؽ� �ϳ��� ũ��.
	unsigned int m_VertexBufferOffset = 0;					// ���ؽ� ������ ������.
	ID3D11Buffer* m_pIndexBuffer = nullptr;					// ���ؽ� ����.
	int m_nIndices = 0;										// �ε��� ����.
	ID3D11ShaderResourceView* m_pTextureRV = nullptr;	// �ؽ�ó ���ҽ� ��.
	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// ���÷� ����.
	D3D11_VIEWPORT viewPort{};
};

