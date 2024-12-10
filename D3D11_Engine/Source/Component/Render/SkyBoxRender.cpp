#include "SkyBoxRender.h"

void SkyBoxRender::Start()
{
    // Front face
    vertices.emplace_back(Vector4(-1.0f, -1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f));
    vertices.emplace_back(Vector4(1.0f, -1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f));
    vertices.emplace_back(Vector4(1.0f, 1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f));
    vertices.emplace_back(Vector4(-1.0f, 1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f));

    // Back face
    vertices.emplace_back(Vector4(-1.0f, -1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f));
    vertices.emplace_back(Vector4(1.0f, -1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f));
    vertices.emplace_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f));
    vertices.emplace_back(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f));

    // Indices for the cube (two triangles per face)
    // Front face
    indices.emplace_back(0); indices.emplace_back(1); indices.emplace_back(2);
    indices.emplace_back(0); indices.emplace_back(2); indices.emplace_back(3);

    // Back face
    indices.emplace_back(4); indices.emplace_back(6); indices.emplace_back(5);
    indices.emplace_back(4); indices.emplace_back(7); indices.emplace_back(6);

    // Left face
    indices.emplace_back(4); indices.emplace_back(5); indices.emplace_back(1);
    indices.emplace_back(4); indices.emplace_back(1); indices.emplace_back(0);

    // Right face
    indices.emplace_back(3); indices.emplace_back(2); indices.emplace_back(6);
    indices.emplace_back(3); indices.emplace_back(6); indices.emplace_back(7);

    // Top face
    indices.emplace_back(1); indices.emplace_back(5); indices.emplace_back(6);
    indices.emplace_back(1); indices.emplace_back(6); indices.emplace_back(2);

    // Bottom face
    indices.emplace_back(4); indices.emplace_back(0); indices.emplace_back(3);
    indices.emplace_back(4); indices.emplace_back(3); indices.emplace_back(7);

    meshResource = std::make_shared<DRAW_INDEX_DATA>();

    CreateMesh();

    textures.resize(1); //큐브맵용 하나만 씀
    ResetSkyBox();

    //SkyBox용 샘플러
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;  // 선형 필터링 (Mipmap 사용 시 선형 보간)
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;   // 좌측, 우측 경계에서 클램프
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;   // 위쪽, 아래쪽 경계에서 클램프
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;   // 큐브 맵의 W축도 클램프
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;  // 비교 함수는 필요하지 않음 (단순 샘플링)
    samplerDesc.BorderColor[0] = 1.0f;                    // 경계 색상 (클램프가 동작할 때 사용)
    samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2] = 1.0f;
    samplerDesc.BorderColor[3] = 1.0f;
    samplerDesc.MipLODBias = 0.0f;                         // Mipmap LOD 바이어스
    samplerDesc.MaxAnisotropy = 1;                         // 비선형 샘플링을 사용하지 않음
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                // Mipmap의 최대 LOD
    samplerDesc.MinLOD = 0.0f;                             // 최소 LOD는 0
    samplerState.resize(1);
    samplerState.SetSamplerState(0, samplerDesc);

    {
        using namespace std::string_literals;
        std::wstring vertexPath(EngineShaderPath + L"SkyBoxVS.hlsl"s);
        SetVertexShader(vertexPath.c_str());

        std::wstring pixelPath(EngineShaderPath + L"SkyBoxPS.hlsl"s);
        SetPixelShader(pixelPath.c_str());
    }
}

void SkyBoxRender::Render()
{
    if (!currPath.empty())
    {
        SimpleMeshRender::Render();
    }
}

void SkyBoxRender::SetSkyBox(const wchar_t* path)
{
    textures.SetCubeMapTexture(0, path);
    currPath = path;
}

void SkyBoxRender::ResetSkyBox()
{
    currPath.clear();
}
