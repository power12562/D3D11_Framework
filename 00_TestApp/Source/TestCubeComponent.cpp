#include "TestCubeComponent.h"
#include <Framework/D3DRenderer.h>

using namespace Utility;

TestCubeComponent::TestCubeComponent()
{

}

TestCubeComponent::~TestCubeComponent()
{
    SafeRelease(m_pIndexBuffer);
    SafeRelease(m_pVertexBuffer);
    SafeRelease(m_pVertexShader);
    SafeRelease(m_pPixelShader);
    SafeRelease(m_pInputLayout);
}

void TestCubeComponent::Start()
{
    struct Vertex
    {
        Vector3 position;		// 정점 위치 정보.
        Vector4 color;			// 정점 색상 정보.
    };

    Vertex vertices[] =
    {
        { Vector3(-1.0f, 1.0f, -1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(1.0f, 1.0f, -1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(1.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(-1.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f) },

        { Vector3(-1.0f, -1.0f, -1.0f), Vector4(0.0f, 0.0f, 0.0, 1.0f) },
        { Vector3(1.0f, -1.0f, -1.0f),	Vector4(0.0f, 0.0f, 0.0, 1.0f) },
        { Vector3(1.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 0.0, 1.0f) },
        { Vector3(-1.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 0.0, 1.0f) },

        { Vector3(-1.0f, -1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0, 1.0f) },
        { Vector3(-1.0f, -1.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0, 1.0f) },
        { Vector3(-1.0f, 1.0f, -1.0f),	Vector4(1.0f, 0.0f, 0.0, 1.0f) },
        { Vector3(-1.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0, 1.0f) },

        { Vector3(1.0f, -1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector3(1.0f, -1.0f, -1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector3(1.0f, 1.0f, -1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector3(1.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f) },

        { Vector3(-1.0f, -1.0f, -1.0f), Vector4(0.0f, 0.0f, 1.0, 1.0f) },
        { Vector3(1.0f, -1.0f, -1.0f),	Vector4(0.0f, 0.0f, 1.0, 1.0f) },
        { Vector3(1.0f, 1.0f, -1.0f),	Vector4(0.0f, 0.0f, 1.0, 1.0f) },
        { Vector3(-1.0f, 1.0f, -1.0f),	Vector4(0.0f, 0.0f, 1.0, 1.0f) },

        { Vector3(-1.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(1.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(1.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(-1.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
    };

    D3D11_BUFFER_DESC bd{};
    bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices; // 배열 데이터 할당.
    CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &m_pVertexBuffer));

    m_VertexBufferStride = sizeof(Vertex);
    m_VertexBufferOffset = 0;

    //인덱스 버퍼용 배열
    UINT indices[] =
    {
       3,1,0, 2,1,3,
       6,4,5, 7,4,6,
       11,9,8, 10,9,11,
       14,12,13, 15,12,14,
       19,17,16, 18,17,19,
       22,20,21, 23,20,22
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
    CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &m_pIndexBuffer));

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    auto pDevice = d3dRenderer.GetDevice();
    ID3D10Blob* vertexShaderBuffer = nullptr;	// 정점 셰이더 코드가 저장될 버퍼.
    CheackHRESULT(CompileShaderFromFile(L"VertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));
    CheackHRESULT(pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
        vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout));

    //버텍스 셰이더 생성
    CheackHRESULT(pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader));
    SafeRelease(vertexShaderBuffer);

    // 픽셀 셰이더 컴파일
    ID3D10Blob* pixelShaderBuffer = nullptr;	// 픽셀 셰이더 코드가 저장될 버퍼.
    CheackHRESULT(CompileShaderFromFile(L"PixelShader.hlsl", "main", "ps_4_0", &pixelShaderBuffer));
    // 픽셸 셰이더 생성
    CheackHRESULT(pDevice->CreatePixelShader(
        pixelShaderBuffer->GetBufferPointer(),
        pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
    SafeRelease(pixelShaderBuffer);
}

void TestCubeComponent::FixedUpdate()
{

}

void TestCubeComponent::Update()
{

}

void TestCubeComponent::LateUpdate()
{

}

void TestCubeComponent::Render()
{
    auto pDeviceContext = d3dRenderer.GetDeviceContext();

    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정점을 이어서 그릴 방식 설정.
    pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &m_VertexBufferOffset);
    pDeviceContext->IASetInputLayout(m_pInputLayout);
    pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	// INDEX값의 범위

    pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);

    pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

    auto* pRenderTargetView = d3dRenderer.GetRenderTargetView();
    pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, d3dRenderer.GetDepthStencilView());  //flip 모드를 사용하기 때문에 매 프레임 설정해주어야 한다.

    pDeviceContext->DrawIndexed(m_nIndices, 0, 0);

}
