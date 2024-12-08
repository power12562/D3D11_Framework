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

        SetVertexShader(L"EngineShader/VertexShader.hlsl");
        SetPixelShader(L"EngineShader/PBRPixelShader.hlsl");
    }
}
