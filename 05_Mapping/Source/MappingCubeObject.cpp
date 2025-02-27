#include "MappingCubeObject.h"
#include <Utility/MemoryUtility.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Manager/HLSLManager.h>
#include <Utility/AssimpUtility.h>

#include "../Source/LightManager.h"

MappingCubeObject::MappingCubeObject()
{
}

MappingCubeObject::~MappingCubeObject()
{
    using namespace Utility;
    SafeRelease(dd.pIndexBuffer);
    SafeRelease(dd.pVertexBuffer);
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
    CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &dd.pVertexBuffer));

    dd.vertexBufferStride = sizeof(Vertex);
    dd.vertexBufferOffset = 0;

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
    dd.indicesCount = ARRAYSIZE(indices);
    bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    //인덱스 버퍼 생성
    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices;
    CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &dd.pIndexBuffer));

    //Load Textures
    textures.resize(5);
    textures.SetTexture2D(E_TEXTURE::Albedo, L"Bricks059_1K-JPG_Color.jpg");
    textures.SetTexture2D(E_TEXTURE::Normal, L"Bricks059_1K-JPG_NormalDX.jpg");
    textures.SetTexture2D(E_TEXTURE::Specular, L"Bricks059_Specular.png");
    textures.SetDefaultTexture(E_TEXTURE::Opacity, E_TEXTURE_DEFAULT::ONE);
    textures.SetDefaultTexture(E_TEXTURE::Emissive, E_TEXTURE_DEFAULT::ZERO);

    hlslManager.CreateSharingShader(L"VertexShader.hlsl", &pVertexShader, &pInputLayout);
    hlslManager.CreateSharingShader(L"PixelShader.hlsl", &pPixelShader);

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
    gameObject.transform.PushUpdateList();
}

void MappingCubeObject::LateUpdate()
{
}

void MappingCubeObject::Render()
{
    auto pDeviceContext = d3dRenderer.GetDeviceContext();

    RENDERER_DRAW_DESC draw_desc{};
    draw_desc.pVertexIndex = &dd;
    draw_desc.pConstBuffer = LightManager::cbuffer.get();
    draw_desc.pD3DTexture2D = &textures;
    draw_desc.pSamperState = &sampler;
    draw_desc.pInputLayout = pInputLayout;
    draw_desc.pVertexShader = pVertexShader;
    draw_desc.pPixelShader = pPixelShader;
    draw_desc.pTransform = &gameObject.transform;
    draw_desc.flags |= RENDER_FORWARD;

    d3dRenderer.DrawIndex(draw_desc);
}

