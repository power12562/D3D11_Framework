#pragma once
#include <Component/Base/RenderComponent.h>

class SimpleUpdateCbuffer : public RenderComponent
{
public:
	SimpleUpdateCbuffer();
	virtual ~SimpleUpdateCbuffer() override;

public:
	virtual void Start() {}
protected:
	virtual void FixedUpdate() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void Render() 		 override;
};