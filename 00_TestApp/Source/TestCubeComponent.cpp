#include "TestCubeComponent.h"
#include <Framework\SceneManager.h>
#include <Framework\TimeSystem.h>
#include <GameObject\Base\CameraObject.h>
#include <Math/Mathf.h>
#include <_Debug/Console.h>

#include "../Source/TestComponent.h"

using namespace Utility;


TestCubeComponent::TestCubeComponent()
{

}

TestCubeComponent::~TestCubeComponent()
{
    using namespace Utility;
    SafeRelease(drawData.pIndexBuffer);
    SafeRelease(drawData.pVertexBuffer);
    SafeRelease(drawData.pPixelShader);
    SafeRelease(drawData.pVertexShader);
    SafeRelease(drawData.pInputLayout);
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
    CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &drawData.pVertexBuffer));

    drawData.vertexBufferStride = sizeof(Vertex);
    drawData.vertexBufferOffset = 0;

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
    drawData.indicesCount = ARRAYSIZE(indices);
    bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    //인덱스 버퍼 생성
    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices;
    CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &drawData.pIndexBuffer));

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    auto pDevice = d3dRenderer.GetDevice();
    ID3D10Blob* vertexShaderBuffer = nullptr;	// 정점 셰이더 코드가 저장될 버퍼.
    CheckHRESULT(CompileShaderFromFile(L"VertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));
    CheckHRESULT(pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
        vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &drawData.pInputLayout));

    //버텍스 셰이더 생성
    CheckHRESULT(pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), NULL, &drawData.pVertexShader));
    SafeRelease(vertexShaderBuffer);

    // 픽셀 셰이더 컴파일
    ID3D10Blob* pixelShaderBuffer = nullptr;	// 픽셀 셰이더 코드가 저장될 버퍼.
    CheckHRESULT(CompileShaderFromFile(L"PixelShader.hlsl", "main", "ps_4_0", &pixelShaderBuffer));
    // 픽셸 셰이더 생성
    CheckHRESULT(pDevice->CreatePixelShader(
        pixelShaderBuffer->GetBufferPointer(),
        pixelShaderBuffer->GetBufferSize(), NULL, &drawData.pPixelShader));
    SafeRelease(pixelShaderBuffer);

    cbuffer.CreatePSConstantBuffers<TestCbuffer>();
}

void TestCubeComponent::FixedUpdate()
{

}

void TestCubeComponent::Update()
{
    elapseTime += TimeSystem::Time.DeltaTime;
    transform.rotation *= Quaternion::CreateFromYawPitchRoll(Vector3(90, 0, 0) * Mathf::Deg2Rad * TimeSystem::Time.DeltaTime);
    transform.localRotation *= Quaternion::CreateFromYawPitchRoll(Vector3(90, 0, 0) * Mathf::Deg2Rad * TimeSystem::Time.DeltaTime);

    if (elapseTime >= 1.5f)
    {
        elapseTime = 0;
    }

    cbuffer.UpdateConstBuffer(testColor);
}

void TestCubeComponent::LateUpdate()
{

}

void TestCubeComponent::Render()
{
    d3dRenderer.DrawIndex(drawData, &cbuffer);
}
