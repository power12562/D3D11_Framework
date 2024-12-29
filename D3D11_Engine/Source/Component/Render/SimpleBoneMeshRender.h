#pragma once
#include <Component/Render/MeshRender.h>
#include <format>

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
		Vector3 Tangent;
		Vector2 Tex;
		int BlendIndecses[4] = {0,};
		float BlendWeights[4] = {0,};
	};
public:
	SimpleBoneMeshRender();
	virtual ~SimpleBoneMeshRender() override = default;
	virtual void Serialized(std::ofstream& ofs);
	virtual void Deserialized(std::ifstream& ifs);

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
	inline std::string GetMatrixPalleteKey();
	std::shared_ptr<MatrixPallete> matrixPallete = {};
	inline std::string GetBoneWITKey();
	std::shared_ptr<BoneWIT> boneWIT = {};

	std::shared_ptr<OffsetMatrices> offsetMatrices = nullptr;
	std::vector<BoneComponent*> boneList;
};

inline std::string SimpleBoneMeshRender::GetMatrixPalleteKey()
{
	return std::format("{}_{}_{}", gameObject.GetNameToString(), gameObject.GetInstanceID(), typeid(MatrixPallete).name()).c_str();
}

inline std::string SimpleBoneMeshRender::GetBoneWITKey()
{
	return std::format("{}_{}_{}", gameObject.GetNameToString(), gameObject.GetInstanceID(), typeid(BoneWIT).name()).c_str();
}

