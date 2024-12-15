#pragma once
#include <GameObject/Mesh/PBRMeshObject.h>

class CubeObject : public PBRMeshObject
{
	SERIALIZED_OBJECT(CubeObject)
public:
	CubeObject();
	virtual ~CubeObject() override = default;
};