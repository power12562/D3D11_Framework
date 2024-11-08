#include "TestCubeComponent.h"
#include <Framework\SceneManager.h>
#include <Framework\TimeSystem.h>
#include <GameObject\Base\CameraObject.h>
#include <Math/Mathf.h>
#include <Utility/Console.h>

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
}

void TestCubeComponent::Start()
{
    struct Vertex
    {
        Vector4 position;		// ���� ��ġ ����.
        Vector4 color;			// ���� ���� ����.
    };

    Vertex vertices[] =
    {
        { Vector4(-1.0f, 1.0f, -1.0f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector4(1.0f, 1.0f, -1.0f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector4(1.0f, 1.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector4(-1.0f, 1.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
                
        { Vector4(-1.0f, -1.0f, -1.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0, 1.0f) },
        { Vector4(1.0f, -1.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 0.0, 1.0f) },
        { Vector4(1.0f, -1.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 0.0, 1.0f) },
        { Vector4(-1.0f, -1.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 0.0, 1.0f) },
                
        { Vector4(-1.0f, -1.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0, 1.0f) },
        { Vector4(-1.0f, -1.0f, -1.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0, 1.0f) },
        { Vector4(-1.0f, 1.0f, -1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0, 1.0f) },
        { Vector4(-1.0f, 1.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0, 1.0f) },
                
        { Vector4(1.0f, -1.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector4(1.0f, -1.0f, -1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector4(1.0f, 1.0f, -1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector4(1.0f, 1.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
                
        { Vector4(-1.0f, -1.0f, -1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0, 1.0f) },
        { Vector4(1.0f, -1.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0, 1.0f) },
        { Vector4(1.0f, 1.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0, 1.0f) },
        { Vector4(-1.0f, 1.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0, 1.0f) },
                
        { Vector4(-1.0f, -1.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector4(1.0f, -1.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector4(1.0f, 1.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector4(-1.0f, 1.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
    };

    D3D11_BUFFER_DESC bd{};
    bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices; // �迭 ������ �Ҵ�.
    CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &drawData.pVertexBuffer));

    drawData.vertexBufferStride = sizeof(Vertex);
    drawData.vertexBufferOffset = 0;

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
    drawData.indicesCount = ARRAYSIZE(indices);
    bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    //�ε��� ���� ����
    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices;
    CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &drawData.pIndexBuffer));

    material.SetVS(L"VertexShader.hlsl");
    material.SetPS(L"PixelShader.hlsl");

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
    material.cbuffer = cbuffer;
}

void TestCubeComponent::LateUpdate()
{

}

void TestCubeComponent::Render()
{
    d3dRenderer.DrawIndex(drawData, material, gameObject.transform);
}
