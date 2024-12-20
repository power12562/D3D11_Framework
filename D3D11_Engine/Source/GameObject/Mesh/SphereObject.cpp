#include "SphereObject.h"
#include <Light/PBRDirectionalLight.h>
#include <Manager/HLSLManager.h>
#include <Utility/AssimpUtility.h>
#include <Component/Render/PBRMeshRender.h>

SphereObject::SphereObject() 
{
	{
		using namespace std::string_literals;
		auto MaterialInit = [this](MeshRender* mesh) {mesh->constBuffer.BindUpdateEvent(Material);};

		std::wstring fbxPath(HLSLManager::EngineShaderPath + L"sphere.fbx"s);
		Utility::LoadFBX(fbxPath.c_str(), *this, MaterialInit, false, SURFACE_TYPE::PBR);
	}	

	for(unsigned int i = 0; i < transform.GetChildCount(); i++)
	{
		SphereMeshRender = transform.GetChild(i)->gameObject.IsComponent<PBRMeshRender>();
		
		if (SphereMeshRender)
		{
			transform.scale = 0.1f;
			break;
		}	
	}
}
