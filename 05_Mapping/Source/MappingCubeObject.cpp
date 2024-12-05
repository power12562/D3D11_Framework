#include "MappingCubeObject.h"
#include <Utility/MemoryUtility.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Framework/HLSLManager.h>
#include <Utility/AssimpUtility.h>

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
    CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &pVertexBuffer));

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
    CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &pIndexBuffer));

    //Load Textures
    texture2D.resize(5);
    texture2D.SetTexture2D(E_TEXTURE::Diffuse, L"Bricks059_1K-JPG_Color.jpg");
    texture2D.SetTexture2D(E_TEXTURE::Normal, L"Bricks059_1K-JPG_NormalDX.jpg");
    texture2D.SetTexture2D(E_TEXTURE::Specular, L"Bricks059_Specular.png");
    texture2D.SetOneTexture(E_TEXTURE::Opacity);
    texture2D.SetZeroTexture(E_TEXTURE::Emissive);

    hlslManager.CreateSharingShader(L"VertexShader.hlsl", "vs_4_0", &pVertexShader, &pInputLayout);
    hlslManager.CreateSharingShader(L"PixelShader.hlsl", "ps_4_0", &pPixelShader);

    //Set sampler
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    sampler.resize(1);
    sampler.SetSamplerState(0, sampDesc);
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

    RENDERER_DRAW_DESC draw_desc{};
    draw_desc.pVertexIndex = &dd;
    draw_desc.pConstBuffer = &LightManager::cbuffer;
    draw_desc.pD3DTexture2D = &texture2D;
    draw_desc.pSamperState = &sampler;
    draw_desc.pInputLayout = pInputLayout;
    draw_desc.pVertexShader = pVertexShader;
    draw_desc.pPixelShader = pPixelShader;
    draw_desc.pTransform = &gameObject.transform;

    d3dRenderer.DrawIndex(draw_desc, false);
}

