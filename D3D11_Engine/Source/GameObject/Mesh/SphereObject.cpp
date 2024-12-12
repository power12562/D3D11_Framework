#include "SphereObject.h"
#include <Light/PBRDirectionalLight.h>

SphereObject::SphereObject() : 
	sphereMeshRender(AddComponent<SphereMeshRender>())
{
	sphereMeshRender.constBuffer.CreatePSConstantBuffers<cb_PBRDirectionalLight>();
	sphereMeshRender.constBuffer.BindUpdateEvent(DirectionalLight::DirectionalLights);

	sphereMeshRender.constBuffer.CreatePSConstantBuffers<cb_PBRMaterial>();
	sphereMeshRender.constBuffer.BindUpdateEvent(Material);

	{
		using namespace std::string_literals;
		std::wstring vertexPath(MeshRender::EngineShaderPath + L"VertexShader.hlsl"s);
		sphereMeshRender.SetVertexShader(vertexPath.c_str());

		std::wstring pixelPath(MeshRender::EngineShaderPath + L"PBRPixelShader.hlsl"s);
		sphereMeshRender.SetPixelShader(pixelPath.c_str());
	}
	sphereMeshRender.CreateSphere(); 
}
