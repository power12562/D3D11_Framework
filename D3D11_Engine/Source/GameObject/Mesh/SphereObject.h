#pragma once
#include <GameObject/Base/GameObject.h>
#include <Material/PBRMaterial.h>
class PBRMeshRender;
class SphereObject : public GameObject
{
	SERIALIZED_OBJECT(SphereObject)
public:
	SphereObject();
	virtual ~SphereObject() override = default;

	cb_PBRMaterial Material;
	PBRMeshRender* SphereMeshRender = nullptr;
};
