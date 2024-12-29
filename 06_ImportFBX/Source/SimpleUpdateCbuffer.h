#pragma once
#include <Component/Base/Component.h>
#include <vector>

#include "../Source/Global_Cbuffer.h"

class SimpleMaterial;
class MeshRender;
class SimpleUpdateCbuffer : public Component
{
public:
	SimpleUpdateCbuffer();
	virtual ~SimpleUpdateCbuffer() override;

public:
	MeshRender* meshRender = nullptr;
	std::shared_ptr<cb_localBool> cb_localbool;

public:
	virtual void Start();
protected:
	virtual void FixedUpdate() {}
	virtual void Update() override;
	virtual void LateUpdate() {}
};