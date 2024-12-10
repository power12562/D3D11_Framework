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

    textures.resize(1); //ť��ʿ� �ϳ��� ��
    ResetSkyBox();

    //SkyBox�� ���÷�
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;  // ���� ���͸� (Mipmap ��� �� ���� ����)
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;   // ����, ���� ��迡�� Ŭ����
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;   // ����, �Ʒ��� ��迡�� Ŭ����
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;   // ť�� ���� W�൵ Ŭ����
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;  // �� �Լ��� �ʿ����� ���� (�ܼ� ���ø�)
    samplerDesc.BorderColor[0] = 1.0f;                    // ��� ���� (Ŭ������ ������ �� ���)
    samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2] = 1.0f;
    samplerDesc.BorderColor[3] = 1.0f;
    samplerDesc.MipLODBias = 0.0f;                         // Mipmap LOD ���̾
    samplerDesc.MaxAnisotropy = 1;                         // ���� ���ø��� ������� ����
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                // Mipmap�� �ִ� LOD
    samplerDesc.MinLOD = 0.0f;                             // �ּ� LOD�� 0
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
