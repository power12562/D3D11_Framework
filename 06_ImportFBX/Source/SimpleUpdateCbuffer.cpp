#include "SimpleUpdateCbuffer.h"
#include <Material\SimpleMaterial.h>
#include <Light\SimpleDirectionalLight.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Framework\ResourceManager.h>

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
	if (meshRender)
	{
		cb_localbool.loaclNormalMap = !!(meshRender->texture2D)[SimpleMaterial::Normal];
		cb_localbool.loaclSpecularMap = !!(meshRender->texture2D)[SimpleMaterial::Specular];
		cb_localbool.loaclEmissiveMap = !!(meshRender->texture2D)[SimpleMaterial::Emissive];
		cb_localbool.loaclOpacityMap = !!(meshRender->texture2D)[SimpleMaterial::Opacity];
		
		SimpleMeshRender* simpleMeshRender = reinterpret_cast<SimpleMeshRender*>(meshRender);
		simpleMeshRender->Material->cb_material = Global_Cbuffer::cb_material;	
	}
}
