#include "MappingCubeObject.h"
#include <Utility/MemoryUtility.h>
#include <Component\Render\SimpleMeshRender.h>

#include "../Source/LightManager.h"

MappingCubeObject::MappingCubeObject()
{
}

MappingCubeObject::~MappingCubeObject()
{
    using namespace Utility;
    SafeRelease(pIndexBuffer);
    SafeRelease(pVertexBuffer);
}

void MappingCubeObject::Start()
{
    using namespace Utility;

    using Vertex = SimpleMeshRender::Vertex;
    Vertex vertices[] =
    {
        { Vector4(-1.0f, 1.0f, -1.0f, 1.f),	Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) },
        { Vector4(1.0f, 1.0f, -1.0f, 1.f),	Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) },
        { Vector4(1.0f, 1.0f, 1.0f, 1.f),	Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) },
        { Vector4(-1.0f, 1.0f, 1.0f, 1.f),	Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) },

        { Vector4(-1.0f, -1.0f, -1.0f, 1.f), Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) },
        { Vector4(1.0f, -1.0f, -1.0f, 1.f),	Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f) },
        { Vector4(1.0f, -1.0f, 1.0f, 1.f),	Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f) },
        { Vector4(-1.0f, -1.0f, 1.0f, 1.f),	Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f) },

        { Vector4(-1.0f, -1.0f, 1.0f, 1.f),	Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f),Vector2(0.0f, 1.0f) },
        { Vector4(-1.0f, -1.0f, -1.0f, 1.f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f),Vector2(1.0f, 1.0f) },
        { Vector4(-1.0f, 1.0f, -1.0f, 1.f),	Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f),Vector2(1.0f, 0.0f) },
        { Vector4(-1.0f, 1.0f, 1.0f, 1.f),	Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f),Vector2(0.0f, 0.0f) },

        { Vector4(1.0f, -1.0f, 1.0f, 1.f),	Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f) },
        { Vector4(1.0f, -1.0f, -1.0f, 1.f),	Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) },
        { Vector4(1.0f, 1.0f, -1.0f, 1.f),	Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) },
        { Vector4(1.0f, 1.0f, 1.0f, 1.f),	Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f) },

        { Vector4(-1.0f, -1.0f, -1.0f, 1.f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f),Vector2(0.0f, 1.0f) },
        { Vector4(1.0f, -1.0f, -1.0f, 1.f),	Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f),Vector2(1.0f, 1.0f) },
        { Vector4(1.0f, 1.0f, -1.0f, 1.f),	Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f),Vector2(1.0f, 0.0f) },
        { Vector4(-1.0f, 1.0f, -1.0f, 1.f),	Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f),Vector2(0.0f, 0.0f) },
                
        { Vector4(-1.0f, -1.0f, 1.0f, 1.f),	Vector3(0.0f, 0.0f, 1.0f),  Vector3(1.0f, 0.0f, 0.0f),Vector2(1.0f, 1.0f) },
        { Vector4(1.0f, -1.0f, 1.0f, 1.f),	Vector3(0.0f, 0.0f, 1.0f),  Vector3(1.0f, 0.0f, 0.0f),Vector2(0.0f, 1.0f) },
        { Vector4(1.0f, 1.0f, 1.0f, 1.f),	Vector3(0.0f, 0.0f, 1.0f),  Vector3(1.0f, 0.0f, 0.0f),Vector2(0.0f, 0.0f) },
        { Vector4(-1.0f, 1.0f, 1.0f, 1.f),	Vector3(0.0f, 0.0f, 1.0f),  Vector3(1.0f, 0.0f, 0.0f),Vector2(1.0f, 0.0f) },
    };

    D3D11_BUFFER_DESC bd{};
    bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices; // 배열 데이터 할당.
    CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &pVertexBuffer));

    vertexBufferStride = sizeof(Vertex);
    vertexBufferOffset = 0;

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
    indicesCount = ARRAYSIZE(indices);
    bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    //인덱스 버퍼 생성
    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices;
    CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &pIndexBuffer));

    //Load Textures
    material.SetDiffuse(L"Bricks059_1K-JPG_Color.jpg");
    material.SetNormalMap(L"Bricks059_1K-JPG_NormalDX.jpg");
    material.SetSpecularMap(L"Bricks059_Specular.png");

    material.SetVS(L"VertexShader.hlsl");

    material.SetPS(L"PixelShader.hlsl");
}

void MappingCubeObject::FixedUpdate()
{
}

void MappingCubeObject::Update()
{
}

void MappingCubeObject::LateUpdate()
{
}

void MappingCubeObject::Render()
{
    auto pDeviceContext = d3dRenderer.GetDeviceContext();

    DRAW_INDEX_DATA dd{};
    dd.indicesCount = indicesCount;
    dd.pIndexBuffer = pIndexBuffer;
    dd.pVertexBuffer = pVertexBuffer;
    dd.vertexBufferOffset = vertexBufferOffset;
    dd.vertexBufferStride = vertexBufferStride;

    material.cbuffer = LightManager::cbuffer;
    d3dRenderer.DrawIndex(dd, material);
}

