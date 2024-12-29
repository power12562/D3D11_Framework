#pragma once
#include <GameObject/Base/GameObject.h>
#include <Material/PBRMaterial.h>
class PBRMeshRender;
class SphereObject : public GameObject
{
	SERIALIZED_OBJECT(SphereObject)
public:
	SphereObject() = default;
	virtual void Start() override;
	virtual ~SphereObject() override = default;

	std::shared_ptr<cb_PBRMaterial> Material;
	PBRMeshRender* SphereMeshRender = nullptr;
};
