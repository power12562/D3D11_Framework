#include "BlingPhongBoneMeshRender.h"
#include <Light/SimpleDirectionalLight.h>

void BlingPhongBoneMeshRender::Start()
{
	SimpleBoneMeshRender::Start();

	BlingPhongMeshObject* meshObj = dynamic_cast<BlingPhongMeshObject*>(&gameObject);
	if (meshObj)
	{
		int index = constBuffer.CreatePSConstantBuffers<cb_BlingPhongMaterial>();
		constBuffer.BindUpdateEvent(meshObj->Material);

		index = constBuffer.CreatePSConstantBuffers<cb_DirectionalLight>();
		constBuffer.BindUpdateEvent(SimpleDirectionalLight::cb_light);

		SetVertexShader(L"EngineShader/VertexSkinningShader.hlsl");
		SetPixelShader(L"EngineShader/BlingPhongPixelShader.hlsl");
	}
}
