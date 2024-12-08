#pragma once
#include <Component/Render/SimpleMeshRender.h>
#include <GameObject/Mesh/PBRMeshObject.h>

class PBRMeshRender : public SimpleMeshRender
{
public:
	PBRMeshRender() = default;
	virtual ~PBRMeshRender() override = default;

	virtual void Start() override;
};