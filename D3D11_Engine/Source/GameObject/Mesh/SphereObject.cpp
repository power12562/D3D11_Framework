#include "SphereObject.h"
#include <Light/PBRDirectionalLight.h>

SphereObject::SphereObject() : 
	sphereMeshRender(AddComponent<SphereMeshRender>())
{
	sphereMeshRender.constBuffer.CreatePSConstantBuffers<cb_PBRDirectionalLight>();
	sphereMeshRender.constBuffer.BindUpdateEvent(PBRDirectionalLight::cb_light);

	sphereMeshRender.constBuffer.CreatePSConstantBuffers<cb_PBRMaterial>();
	sphereMeshRender.constBuffer.BindUpdateEvent(Material);

	sphereMeshRender.SetVertexShader(L"EngineShader/VertexShader.hlsl");
	sphereMeshRender.SetPixelShader(L"EngineShader/PBRPixelShader.hlsl");

	sphereMeshRender.CreateSphere();
}
