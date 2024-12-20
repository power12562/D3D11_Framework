#include "PBRMeshRender.h"
#include <Light/PBRDirectionalLight.h>
#include <Manager/HLSLManager.h>

PBRMeshRender::PBRMeshRender()
{
    
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

            std::wstring pixelPath(HLSLManager::EngineShaderPath + L"PBRPixelShader.hlsl"s);
            SetPixelShader(pixelPath.c_str());
        }
    }
}
