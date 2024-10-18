#pragma once
#include <Component/Base/RenderComponent.h>
#include <vector>

#include "../Source/Global_Cbuffer.h"

class SimpleMaterial;
class SimpleUpdateCbuffer : public RenderComponent
{
public:
	SimpleUpdateCbuffer();
	virtual ~SimpleUpdateCbuffer() override;

public:
	SimpleMaterial* Material = nullptr;
	cb_localBool cb_localbool;

public:
	virtual void Start();
protected:
	virtual void FixedUpdate() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void Render() 		 override;
};