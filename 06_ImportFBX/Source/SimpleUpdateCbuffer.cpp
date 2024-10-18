#include "SimpleUpdateCbuffer.h"
#include <Material\SimpleMaterial.h>
#include <Light\SimpleDirectionalLight.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Framework\MaterialManager.h>

#include "../Source/Global_Cbuffer.h"

SimpleUpdateCbuffer::SimpleUpdateCbuffer()
{
	
}

SimpleUpdateCbuffer::~SimpleUpdateCbuffer()
{
	
}

void SimpleUpdateCbuffer::Render()
{
	auto* Material = materialManager.GetMaterial(myKey.c_str());

	Global_Cbuffer::cb_localbool.loaclNormalMap = Material->IsNormalMap();
	Global_Cbuffer::cb_localbool.loaclSpecularMap = Material->IsSpecularMap();
	Global_Cbuffer::cb_localbool.loaclEmissiveMap = Material->IsEmissiveMap();
	Global_Cbuffer::cb_localbool.loaclOpacityMap = Material->IsOpacityMap();

	Material->cbuffer.UpdateConstBuffer(Global_Cbuffer::cb_bool);
	Material->cbuffer.UpdateConstBuffer(Global_Cbuffer::cb_localbool);

	Material->cb_material = Global_Cbuffer::cb_material;
}
