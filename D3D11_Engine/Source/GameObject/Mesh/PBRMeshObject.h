#pragma once
#include <GameObject/Base/GameObject.h>
#include <Material/PBRMaterial.h>

class PBRMeshObject : public GameObject
{
	SERIALIZED_OBJECT(PBRMeshObject)
public:
	PBRMeshObject() = default;
	virtual ~PBRMeshObject() override = default;
	virtual void Serialized(std::ofstream& ofs);
	virtual void Deserialized(std::ifstream& ifs);

	cb_PBRMaterial Material;
};