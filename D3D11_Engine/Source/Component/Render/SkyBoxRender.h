#pragma once
#include <Component/Render/MeshRender.h>

class SkyBoxRender : public MeshRender
{
	struct Vertex
	{
		Vector4 position{ 0,0,0,1 };
		Vector3 normal;
		Vector3 biTangent;
		Vector2 Tex;
	};
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
public:
	SkyBoxRender() = default;
	virtual ~SkyBoxRender() override = default;

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
};