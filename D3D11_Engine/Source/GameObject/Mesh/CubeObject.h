#pragma once
#include <GameObject/Mesh/PBRMeshObject.h>

class CubeObject : public PBRMeshObject
{
public:
	CubeObject();
	virtual ~CubeObject() override = default;
};