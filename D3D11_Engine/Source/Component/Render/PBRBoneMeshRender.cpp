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


        {
            using namespace std::string_literals;
            std::wstring vertexPath(EngineShaderPath + L"VertexSkinningShader.hlsl"s);
            SetVertexShader(vertexPath.c_str());

            std::wstring pixelPath(EngineShaderPath + L"PBRPixelShader.hlsl"s);
            SetPixelShader(pixelPath.c_str());
        }
    }
}
