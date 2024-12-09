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

		{
			using namespace std::string_literals;
			std::wstring vertexPath(EngineShaderPath + L"VertexSkinningShader.hlsl"s);
			SetVertexShader(vertexPath.c_str());

			std::wstring pixelPath(EngineShaderPath + L"BlingPhongPixelShader.hlsl"s);
			SetPixelShader(pixelPath.c_str());
		}
	}
}
