#pragma once
#include <Component/Render/SimpleMeshRender.h>

class SkyBoxRender : public SimpleMeshRender
{
public:
	SkyBoxRender() = default;
	virtual ~SkyBoxRender() override = default;

	virtual void Start() override;

public:
	void SetSkyBox(const wchar_t* path);
	void ResetSkyBox();

private:
	using SimpleMeshRender::CreateMesh;
};