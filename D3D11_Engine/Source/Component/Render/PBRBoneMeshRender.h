#pragma once
#include <Component/Render/SimpleBoneMeshRender.h>
#include <GameObject/Mesh/PBRMeshObject.h>

class PBRBoneMeshRender : public SimpleBoneMeshRender
{
public:
	PBRBoneMeshRender() = default;
	virtual ~PBRBoneMeshRender() override = default;

	virtual void Start() override;
};