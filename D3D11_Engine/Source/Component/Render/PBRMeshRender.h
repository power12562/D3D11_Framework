#pragma once
#include <Component/Render/SimpleMeshRender.h>
#include <GameObject/Mesh/PBRMeshObject.h>

class PBRMeshRender : public SimpleMeshRender
{
public:
	PBRMeshRender();
	virtual ~PBRMeshRender() override = default;
	virtual void Serialized(std::ofstream& ofs);
	virtual void Deserialized(std::ifstream& ifs);

	virtual void Start() override;
};