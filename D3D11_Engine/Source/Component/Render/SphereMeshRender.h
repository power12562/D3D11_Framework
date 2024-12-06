#pragma once
#include <Component/Render/SimpleMeshRender.h>

class SphereMeshRender : public SimpleMeshRender
{
public:
	SphereMeshRender();
	virtual ~SphereMeshRender() override = default;

	void CreateSphere(float radius = 1.f, UINT stackCount = 30, UINT sliceCount = 30);
protected:
	using SimpleMeshRender::CreateMesh;
};