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
}

SimpleMashComponent::~SimpleMashComponent()
{
	delete Material;
}

void SimpleMashComponent::Render()
{
	Material->cbuffer.UpdateConstBuffer(SimpleDirectionalLight::cb_Light);
	Material->cb_material = SimpleDirectionalLight::cb_material;
	__super::Render();
}
