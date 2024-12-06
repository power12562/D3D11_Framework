#pragma once
#include <GameObject/Base/GameObject.h>
class SphereMeshRender;
class SphereObject : public GameObject
{
public:
	SphereObject();
	virtual ~SphereObject() override = default;

	SphereMeshRender& sphereMeshRender;
};
#include <Component/Render/SphereMeshRender.h>