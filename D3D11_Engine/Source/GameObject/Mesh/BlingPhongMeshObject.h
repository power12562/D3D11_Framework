#pragma once
#include <GameObject/Base/GameObject.h>
#include <Material/BlingPhongMaterial.h>

class BlingPhongMeshObject : public GameObject
{
	SERIALIZED_OBJECT(BlingPhongMeshObject)
public:
	BlingPhongMeshObject() = default;
	virtual ~BlingPhongMeshObject() override = default;
	virtual void Start() override;

	std::shared_ptr<cb_BlingPhongMaterial> Material;
};