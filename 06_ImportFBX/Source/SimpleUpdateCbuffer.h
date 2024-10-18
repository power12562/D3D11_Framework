#pragma once
#include <Component/Base/RenderComponent.h>
#include <vector>

class SimpleMaterial;
class SimpleUpdateCbuffer : public RenderComponent
{
public:
	SimpleUpdateCbuffer();
	virtual ~SimpleUpdateCbuffer() override;

public:
	std::vector<SimpleMaterial*> list;

public:
	virtual void Start() {}
protected:
	virtual void FixedUpdate() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void Render() 		 override;
};