#include "SimpleMashComponent.h"
#include <Material\SimpleMaterial.h>

#include "../Source/SimpleDirectionalLight.h"

SimpleMashComponent::SimpleMashComponent()
{
	Material = new SimpleMaterial;

	Material->SetVS(L"VertexShader.hlsl");
	Material->SetPS(L"PixelShader.hlsl");

	int index = -1;
	index = Material->cbuffer.CreatePSConstantBuffers<cbuffer_Light>();
	index = Material->cbuffer.CreatePSConstantBuffers<cbuffer_bool>();
	index = Material->cbuffer.CreatePSConstantBuffers<cb_localBool>();
}

SimpleMashComponent::~SimpleMashComponent()
{
	delete Material;
}

void SimpleMashComponent::Render()
{
	SimpleDirectionalLight::cb_localbool.loaclNormalMap = Material->IsNormalMap();
	SimpleDirectionalLight::cb_localbool.loaclSpecularMap = Material->IsSpecularMap();
	SimpleDirectionalLight::cb_localbool.loaclEmissiveMap = Material->IsEmissiveMap();
	SimpleDirectionalLight::cb_localbool.loaclOpacityMap = Material->IsOpacityMap();

	Material->cbuffer.UpdateConstBuffer(SimpleDirectionalLight::cb_Light);
	Material->cbuffer.UpdateConstBuffer(SimpleDirectionalLight::cb_bool);
	Material->cbuffer.UpdateConstBuffer(SimpleDirectionalLight::cb_localbool);

	Material->cb_material = SimpleDirectionalLight::cb_material;
	__super::Render();
}
