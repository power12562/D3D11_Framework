#pragma once
#include <GameObject/Base/GameObject.h>
#include <Material/PBRMaterial.h>

class PBRMeshObject : public GameObject
{
	SERIALIZED_OBJECT(PBRMeshObject)
public:
	PBRMeshObject() = default;
	virtual ~PBRMeshObject() override = default;

	cb_PBRMaterial Material;
};