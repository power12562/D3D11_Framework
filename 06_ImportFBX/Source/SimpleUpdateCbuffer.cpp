#include "SimpleUpdateCbuffer.h"
#include <Material\SimpleMaterial.h>
#include <Light\SimpleDirectionalLight.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Framework\ResourceManager.h>
#include <Utility/AssimpUtility.h>

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
		cb_localbool.loaclNormalMap = !!(meshRender->texture2D)[E_TEXTURE::Normal];
		cb_localbool.loaclSpecularMap = !!(meshRender->texture2D)[E_TEXTURE::Specular];
		cb_localbool.loaclEmissiveMap = !!(meshRender->texture2D)[E_TEXTURE::Emissive];
		cb_localbool.loaclOpacityMap = !!(meshRender->texture2D)[E_TEXTURE::Opacity];
		
		SimpleMeshRender* simpleMeshRender = reinterpret_cast<SimpleMeshRender*>(meshRender);
	}
}
