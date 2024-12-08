#include "PBRBoneMeshRender.h"
#include <Light/PBRDirectionalLight.h>

void PBRBoneMeshRender::Start()
{
	SimpleBoneMeshRender::Start();
    PBRMeshObject* meshObj = dynamic_cast<PBRMeshObject*>(&gameObject);
    if (meshObj)
    {
        int index = constBuffer.CreatePSConstantBuffers<cb_PBRDirectionalLight>();
        constBuffer.BindUpdateEvent(PBRDirectionalLight::cb_light);

        index = constBuffer.CreatePSConstantBuffers<cb_PBRMaterial>();
        constBuffer.BindUpdateEvent(meshObj->Material);

        SetVertexShader(L"EngineShader/VertexSkinningShader.hlsl");
        SetPixelShader(L"EngineShader/PBRPixelShader.hlsl");
    }
}
