#pragma once
#include <Component/Render/SimpleMeshRender.h>

class SimpleMashComponent : public SimpleMeshRender
{
public:
	SimpleMashComponent();
	virtual ~SimpleMashComponent() override;

public:
	virtual void Render() 		 override;
};