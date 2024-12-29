#include "SphereObject.h"
#include <Light/PBRDirectionalLight.h>
#include <Manager/HLSLManager.h>
#include <Utility/AssimpUtility.h>
#include <Component/Render/PBRMeshRender.h>

void SphereObject::Start()
{
	{
		using namespace std::string_literals;
		Material = D3DConstBuffer::GetData<cb_PBRMaterial>(GetNameToString().c_str());

		std::wstring fbxPath(HLSLManager::EngineShaderPath + L"sphere.fbx"s);
		Utility::LoadFBX(fbxPath.c_str(), *this, false, SURFACE_TYPE::PBR);
	}

	for (unsigned int i = 0; i < transform.GetChildCount(); i++)
	{
		SphereMeshRender = transform.GetChild(i)->gameObject.IsComponent<PBRMeshRender>();

		if (SphereMeshRender)
		{
			transform.scale = 0.1f;
			break;
		}
	}
}
