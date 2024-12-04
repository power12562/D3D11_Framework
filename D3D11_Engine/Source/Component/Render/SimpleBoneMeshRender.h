#pragma once
#include <Component/Render/MeshRender.h>

struct MatrixPallete
{
	Matrix MatrixPalleteArray[128];
};

struct BoneWIT
{
	Matrix BoneWIT[128];
};

struct OffsetMatrices
{
	std::vector<Matrix> data;
};

class BoneComponent;
class SimpleMaterial;
class SimpleBoneMeshRender : public MeshRender
{
public:
	struct Vertex
	{
		Vector4 position{ 0,0,0,1 };
		Vector3 normal;
		Vector3 biTangent;
		Vector2 Tex;
		int BlendIndecses[4] = {0,};
		float BlendWeights[4] = {0,};
	};
public:
	SimpleBoneMeshRender();
	virtual ~SimpleBoneMeshRender() override = default;

public:
	virtual void Start() 		 override;
protected:
	virtual void FixedUpdate()	 override;
	virtual void Update() 		 override;
	virtual void LateUpdate()	 override;
	virtual void Render() 		 override;

public:
	virtual void CreateMesh() override;

public:
	std::vector<Vertex> vertices;
	std::vector<UINT>   indices;

public:
	std::shared_ptr<SimpleMaterial> Material = nullptr;

public:
	MatrixPallete matrixPallete = {};
	BoneWIT boneWIT = {};
	std::shared_ptr<OffsetMatrices> offsetMatrices = nullptr;
	std::vector<BoneComponent*> boneList;
};