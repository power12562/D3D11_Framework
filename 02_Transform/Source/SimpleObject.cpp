#include "SimpleObject.h"
#include <Utility/D3D11Utility.h>
#include <Utility\MemoryUtility.h>
#include "TransformTest.h"    

using namespace Utility;

SimpleObject::SimpleObject()
{
    transform = new Transform;
}

SimpleObject::~SimpleObject()
{
    delete transform;
}

CubeObject::CubeObject()
{
    Vertex vertices[] =
    {
        { Vector3(-1.0f, 1.0f, -1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(1.0f, 1.0f, -1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(1.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
        { Vector3(-1.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector3(-1.0f, -1.0f, -1.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(1.0f, -1.0f, -1.0f),	Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(1.0f, -1.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vector3(-1.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 0.0f, 1.0f) },
    };

    D3D11_BUFFER_DESC bd{};
    bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices; // 배열 데이터 할당.
    CheackHRESULT(TransformTest::GetDevice()->CreateBuffer(&bd, &vbData, &m_pVertexBuffer));

    m_VertexBufferStride = sizeof(Vertex);
    m_VertexBufferOffset = 0;

    //인덱스 버퍼용 배열
    UINT indices[] =
    {
        3,1,0, 2,1,3,
        0,5,4, 1,5,0,
        3,4,7, 0,4,3,
        1,6,5, 2,6,1,
        2,7,6, 3,7,2,
        6,4,5, 7,4,6,
    };
    // 인덱스 개수 저장.
    m_nIndices = ARRAYSIZE(indices);
    bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    //인덱스 버퍼 생성
    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices;
    CheackHRESULT(TransformTest::GetDevice()->CreateBuffer(&bd, &ibData, &m_pIndexBuffer));
}

CubeObject::~CubeObject()
{
    SafeRelease(m_pIndexBuffer);
    SafeRelease(m_pVertexBuffer);
}

void CubeObject::Update()
{
    transform->UpdateTransform();
    m_World = transform->GetWM();
}

void CubeObject::Render(ID3D11Buffer* pConstantBuffer, ID3D11InputLayout* pInputLayout, ID3D11VertexShader* pVertexShader, ID3D11PixelShader* pPixelShader)
{
    auto pDeviceContext = TransformTest::GetDeviceContext();

    // 상수버퍼 업데이트
    ConstantBuffer cb;
    cb.mWorld = XMMatrixTranspose(m_World);
    cb.mView = XMMatrixTranspose(SimpleObject::mView);
    cb.mProjection = XMMatrixTranspose(SimpleObject::mProjection);
    cb.WVP = XMMatrixTranspose(m_World * SimpleObject::mView * SimpleObject::mProjection);

    pDeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);

    // Draw계열 함수를 호출하기전에 렌더링 파이프라인에 필수 스테이지 설정을 해야한다.	
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정점을 이어서 그릴 방식 설정.
    pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &m_VertexBufferOffset);
    pDeviceContext->IASetInputLayout(pInputLayout);
    pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	// INDEX값의 범위
    pDeviceContext->VSSetShader(pVertexShader, nullptr, 0);
    pDeviceContext->PSSetShader(pPixelShader, nullptr, 0);
    pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

    pDeviceContext->DrawIndexed(m_nIndices, 0, 0);
}

