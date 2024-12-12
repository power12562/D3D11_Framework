#include "PBRMeshRender.h"
#include <Light/PBRDirectionalLight.h>

void PBRMeshRender::Start()
{
    SimpleMeshRender::Start();

    //BRDF LookUp Table Sampler
    samplerState.resize(2);
    D3D11_SAMPLER_DESC LUTSamplerDesc = {};
    LUTSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    LUTSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    LUTSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    LUTSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    LUTSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    LUTSamplerDesc.MinLOD = 0;
    LUTSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerState.SetSamplerState(1, LUTSamplerDesc);

    PBRMeshObject* meshObj = dynamic_cast<PBRMeshObject*>(&gameObject);
    if (meshObj)
    {
        int index = constBuffer.CreatePSConstantBuffers<cb_PBRDirectionalLight>();
        constBuffer.BindUpdateEvent(PBRDirectionalLight::cb_light);

        index = constBuffer.CreatePSConstantBuffers<cb_PBRMaterial>();
        constBuffer.BindUpdateEvent(meshObj->Material);

        {
            using namespace std::string_literals;
            std::wstring vertexPath(EngineShaderPath + L"VertexShader.hlsl"s);
            SetVertexShader(vertexPath.c_str());

            std::wstring pixelPath(EngineShaderPath + L"PBRPixelShader.hlsl"s);
            SetPixelShader(pixelPath.c_str());
        }
    }
}
