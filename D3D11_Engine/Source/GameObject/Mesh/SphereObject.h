#pragma once
#include <GameObject/Base/GameObject.h>
#include <Component/Render/SphereMeshRender.h>
#include <Material/PBRMaterial.h>
class SphereObject : public GameObject
{
public:
	SphereObject();
	virtual ~SphereObject() override = default;

	SphereMeshRender& sphereMeshRender;
	cb_PBRMaterial Material;
};
