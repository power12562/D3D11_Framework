#pragma once
#include <GameObject/Base/GameObject.h>
#include <Material/BlingPhongMaterial.h>

class BlingPhongMeshObject : public GameObject
{
public:
	BlingPhongMeshObject() = default;
	virtual ~BlingPhongMeshObject() override = default;

	cb_BlingPhongMaterial Material;
};