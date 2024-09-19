#include "SimpleShadeCube.h"
#include "ShadeTestApp.h"

#include <Framework\D3DRenderer.h>
#include <Utility/D3D11Utility.h>
#include <Utility\MemoryUtility.h>

using namespace Utility;

SimpleShadeObject::SimpleShadeObject()
{
    transform = new Transform;
}

SimpleShadeObject::~SimpleShadeObject()
{
    delete transform;
}

SimpleCubeShadeObject::SimpleCubeShadeObject()
{
    Vertex vertices[] =
    {
       { Vector3(-1.0f, 1.0f, -1.0f),	Vector3(0.0f, 1.0f, 0.0f) },// Normal Y +	 
        { Vector3(1.0f, 1.0f, -1.0f),	Vector3(0.0f, 1.0f, 0.0f) },
        { Vector3(1.0f, 1.0f, 1.0f),	Vector3(0.0f, 1.0f, 0.0f) },
        { Vector3(-1.0f, 1.0f, 1.0f),	Vector3(0.0f, 1.0f, 0.0f) },

        { Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f) },// Normal Y -		
        { Vector3(1.0f, -1.0f, -1.0f),	Vector3(0.0f, -1.0f, 0.0f) },
        { Vector3(1.0f, -1.0f, 1.0f),	Vector3(0.0f, -1.0f, 0.0f) },
        { Vector3(-1.0f, -1.0f, 1.0f),	Vector3(0.0f, -1.0f, 0.0f) },

        { Vector3(-1.0f, -1.0f, 1.0f),	Vector3(-1.0f, 0.0f, 0.0f) },//	Normal X -
        { Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f) },
        { Vector3(-1.0f, 1.0f, -1.0f),	Vector3(-1.0f, 0.0f, 0.0f) },
        { Vector3(-1.0f, 1.0f, 1.0f),	Vector3(-1.0f, 0.0f, 0.0f) },

        { Vector3(1.0f, -1.0f, 1.0f),	Vector3(1.0f, 0.0f, 0.0f) },// Normal X +
        { Vector3(1.0f, -1.0f, -1.0f),	Vector3(1.0f, 0.0f, 0.0f) },
        { Vector3(1.0f, 1.0f, -1.0f),	Vector3(1.0f, 0.0f, 0.0f) },
        { Vector3(1.0f, 1.0f, 1.0f),	Vector3(1.0f, 0.0f, 0.0f) },

        { Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f) }, // Normal Z -
        { Vector3(1.0f, -1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f) },
        { Vector3(1.0f, 1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f) },
        { Vector3(-1.0f, 1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f) },

        { Vector3(-1.0f, -1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f) },// Normal Z +
        { Vector3(1.0f, -1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f) },
        { Vector3(1.0f, 1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f) },
        { Vector3(-1.0f, 1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f) },
    };

    D3D11_BUFFER_DESC bd{};
    bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices; // �迭 ������ �Ҵ�.
    CheackHRESULT(D3D11Renderer.GetDevice()->CreateBuffer(&bd, &vbData, &m_pVertexBuffer));

    m_VertexBufferStride = sizeof(Vertex);
    m_VertexBufferOffset = 0;

    //�ε��� ���ۿ� �迭
    UINT indices[] =
    {
       3,1,0, 2,1,3,
       6,4,5, 7,4,6,
       11,9,8, 10,9,11,
       14,12,13, 15,12,14,
       19,17,16, 18,17,19,
       22,20,21, 23,20,22
    };

    // �ε��� ���� ����.
    m_nIndices = ARRAYSIZE(indices);
    bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    //�ε��� ���� ����
    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices;
    CheackHRESULT(D3D11Renderer.GetDevice()->CreateBuffer(&bd, &ibData, &m_pIndexBuffer));
}

SimpleCubeShadeObject::~SimpleCubeShadeObject()
{
    SafeRelease(m_pIndexBuffer);
    SafeRelease(m_pVertexBuffer);
}

void SimpleCubeShadeObject::Update()
{
    transform->UpdateTransform();
}

void SimpleCubeShadeObject::Render(ID3D11Buffer* pConstantBuffer, ID3D11InputLayout* pInputLayout, ID3D11VertexShader* pVertexShader, ID3D11PixelShader* pPixelShader)
{
    auto pDeviceContext = D3D11Renderer.GetDeviceContext();

    // ������� ������Ʈ
    ConstantBuffer cb;
    cb.mWorld = XMMatrixTranspose(transform->GetWM());
    cb.mView = XMMatrixTranspose(SimpleCubeShadeObject::mView);
    cb.mProjection = XMMatrixTranspose(SimpleCubeShadeObject::mProjection);
    cb.WVP = XMMatrixTranspose(transform->GetWM() * SimpleCubeShadeObject::mView * SimpleCubeShadeObject::mProjection);
    cb.vLightDir = Vector4(0, 0, 1, 1);
    cb.vLightColor = Vector4(0, 0, 0, 1);

    pDeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);

    // Draw�迭 �Լ��� ȣ���ϱ����� ������ ���������ο� �ʼ� �������� ������ �ؾ��Ѵ�.	
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ������ �̾ �׸� ��� ����.
    pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &m_VertexBufferOffset);
    pDeviceContext->IASetInputLayout(pInputLayout);
    pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	// INDEX���� ����
    pDeviceContext->VSSetShader(pVertexShader, nullptr, 0);
    pDeviceContext->PSSetShader(pPixelShader, nullptr, 0);
    pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

    pDeviceContext->DrawIndexed(m_nIndices, 0, 0);
}

