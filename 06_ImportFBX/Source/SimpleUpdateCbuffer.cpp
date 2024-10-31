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

void SimpleUpdateCbuffer::Start()
{
	
}

void SimpleUpdateCbuffer::Update()
{
	if (Material)
	{
		cb_localbool.loaclNormalMap = Material->IsNormalMap();
		cb_localbool.loaclSpecularMap = Material->IsSpecularMap();
		cb_localbool.loaclEmissiveMap = Material->IsEmissiveMap();
		cb_localbool.loaclOpacityMap = Material->IsOpacityMap();

		Material->cb_material = Global_Cbuffer::cb_material;
	}
}
