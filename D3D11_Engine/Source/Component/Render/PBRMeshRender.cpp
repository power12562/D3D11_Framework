#include "PBRMeshRender.h"
#include <Light/PBRDirectionalLight.h>

void PBRMeshRender::Start()
{
    SimpleMeshRender::Start();

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
