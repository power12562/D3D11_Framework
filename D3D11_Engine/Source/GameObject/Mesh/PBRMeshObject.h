#pragma once
#include <GameObject/Base/GameObject.h>
#include <Material/PBRMaterial.h>

class PBRMeshObject : public GameObject
{
public:
	PBRMeshObject() = default;
	virtual ~PBRMeshObject() override = default;

	cb_PBRMaterial Material;
};