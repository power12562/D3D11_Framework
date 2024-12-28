#include "PBRMeshRender.h"
#include <Light/PBRDirectionalLight.h>
#include <Manager/HLSLManager.h>
#include <Utility/SerializedUtility.h>

PBRMeshRender::PBRMeshRender()
{
    
}

void PBRMeshRender::Serialized(std::ofstream& ofs)
{
    using namespace Binary;
    Write::data(ofs, Enable);
    Write::Color(ofs, baseColor);
    Write::data(ofs, RenderFlags);
    Write::data(ofs, MeshID);
    Write::wstring(ofs, GetVertexShaderPath());
    Write::wstring(ofs, GetPixelShaderPath());

	D3D11_BUFFER_DESC bd{};
	meshResource->pIndexBuffer->GetDesc(&bd);
	indices.resize(bd.ByteWidth / sizeof(UINT));
	Utility::RetrieveIndexBufferData(d3dRenderer.GetDeviceContext(), d3dRenderer.GetDevice(), meshResource->pIndexBuffer, indices.data(), bd.ByteWidth);
	Write::data(ofs, indices.size());
    ofs.write(reinterpret_cast<const char*>(indices.data()), bd.ByteWidth);
    indices.clear(); 
    Write::data(ofs, meshResource->indicesCount);

    meshResource->pVertexBuffer->GetDesc(&bd);
    vertices.resize(bd.ByteWidth / sizeof(Vertex));
    Utility::RetrieveVertexBufferData(d3dRenderer.GetDeviceContext(), d3dRenderer.GetDevice(), meshResource->pVertexBuffer, vertices.data(), bd.ByteWidth);
    Write::data(ofs, vertices.size());
    ofs.write(reinterpret_cast<const char*>(vertices.data()), bd.ByteWidth);
    vertices.clear();
    Write::data(ofs, meshResource->vertexBufferOffset);
    Write::data(ofs, meshResource->vertexBufferStride);

    textures.Serialized(ofs);
}

void PBRMeshRender::Deserialized(std::ifstream& ifs)
{
    using namespace Binary;
    Enable = Read::data<bool>(ifs);
    baseColor = Read::Color(ifs);
    RenderFlags = Read::data<int>(ifs);
    MeshID = Read::data<int>(ifs);   SetMeshResource(gameObject.Name.c_str());
    SetVertexShader(Read::wstring(ifs).c_str());
    SetPixelShader(Read::wstring(ifs).c_str());

    size_t indicesSize = Read::data<size_t>(ifs);
    indices.resize(indicesSize);
    ifs.read(reinterpret_cast<char*>(indices.data()), sizeof(decltype(indices[0])) * indicesSize);
    if (meshResource->pIndexBuffer != nullptr) indices.clear();
    meshResource->indicesCount = Read::data<int>(ifs);

    size_t verticesSize = Read::data<size_t>(ifs);
    vertices.resize(verticesSize);
    ifs.read(reinterpret_cast<char*>(vertices.data()), sizeof(Vertex) * verticesSize);
    if (meshResource->pVertexBuffer != nullptr) vertices.clear();
    meshResource->vertexBufferOffset = Read::data<decltype(meshResource->vertexBufferOffset)>(ifs);
    meshResource->vertexBufferStride = Read::data<decltype(meshResource->vertexBufferStride)>(ifs);

    if (!indices.empty() && !vertices.empty())
        CreateMesh();

    textures.Deserialized(ifs);
}

void PBRMeshRender::Start()
{
    SimpleMeshRender::Start();

    //BRDF LookUp Table Sampler
    samplerState.resize(3);
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerState.SetSamplerState(1, sampDesc);

    //Shadow Sampler
    sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerState.SetSamplerState(2, sampDesc);

    PBRMeshObject* meshObj = dynamic_cast<PBRMeshObject*>(&gameObject);
    if (meshObj)
    {
        int index = constBuffer.CreatePSConstantBuffers<cb_PBRDirectionalLight>();
        constBuffer.BindUpdateEvent(DirectionalLight::DirectionalLights);

        index = constBuffer.CreatePSConstantBuffers<cb_PBRMaterial>();
        constBuffer.BindUpdateEvent(meshObj->Material);

        {
            using namespace std::string_literals;
            std::wstring vertexPath(HLSLManager::EngineShaderPath + L"VertexShader.hlsl"s);
            SetVertexShader(vertexPath.c_str());

            std::wstring pixelPath(HLSLManager::EngineShaderPath + L"PBROpaquePS.hlsl"s);
            SetPixelShader(pixelPath.c_str());
        }
    }
}
