#pragma once
#include <Component/Render/MeshRender.h>

class SkyBoxRender : public MeshRender
{
	inline static SkyBoxRender* mainSkyBox = nullptr;
public:
	inline static SkyBoxRender* GetMainSkyBox() { return mainSkyBox; }

public:
	SkyBoxRender() = default;
	virtual ~SkyBoxRender() override;

	virtual void Start() override;
protected:
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	void SetSkyBox(const wchar_t* path);
	void ResetSkyBox();

private:
	virtual void CreateMesh() override;

private:
	std::wstring currPath;

	std::vector<Vector4> vertices;
	std::vector<UINT> indices;
};