#include "SphereObject.h"
#include <Light/PBRDirectionalLight.h>
#include <Manager/HLSLManager.h>
#include <Utility/AssimpUtility.h>
#include <Component/Render/PBRMeshRender.h>
#include <filesystem>

void SphereObject::Start()
{
	{
		using namespace std::string_literals;
		Material = D3DConstBuffer::GetData<cb_PBRMaterial>(GetNameToString().c_str());

		std::wstring fbxPath(HLSLManager::EngineShaderPath + L"sphere.fbx"s);
		std::filesystem::path objectPath(HLSLManager::EngineShaderPath + L"sphere.GameObject"s);
		if (!std::filesystem::exists(objectPath))
			Utility::LoadFBX(fbxPath.c_str(), false, SURFACE_TYPE::PBR);
		else
			gameObjectFactory.DeserializedObject(objectPath.c_str());
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
