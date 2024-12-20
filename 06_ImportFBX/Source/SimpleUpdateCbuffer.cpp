#include "SimpleUpdateCbuffer.h"
#include <Material\BlingPhongMaterial.h>
#include <Light\SimpleDirectionalLight.h>
#include <Component\Render\SimpleMeshRender.h>
#include <Manager/ResourceManager.h>
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
		cb_localbool.loaclNormalMap = !!(meshRender->textures)[E_TEXTURE::Normal];
		cb_localbool.loaclSpecularMap = !!(meshRender->textures)[E_TEXTURE::Specular];
		cb_localbool.loaclEmissiveMap = !!(meshRender->textures)[E_TEXTURE::Emissive];
		cb_localbool.loaclOpacityMap = !!(meshRender->textures)[E_TEXTURE::Opacity];
		
		SimpleMeshRender* simpleMeshRender = reinterpret_cast<SimpleMeshRender*>(meshRender);
	}
}
