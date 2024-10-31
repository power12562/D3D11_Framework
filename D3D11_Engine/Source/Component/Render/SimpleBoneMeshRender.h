#pragma once
#include <Component/Base/RenderComponent.h>

struct MatrixPallete
{
	Matrix MatrixPalleteArray[128];
};

struct BoneWIT
{
	Matrix BoneWIT[128];
};

class BoneComponent;
class SimpleMaterial;
class SimpleBoneMeshRender : public RenderComponent
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
	virtual ~SimpleBoneMeshRender() override;

public:
	virtual void Start() 		 override;
protected:
	virtual void FixedUpdate()	 override;
	virtual void Update() 		 override;
	virtual void LateUpdate()	 override;
	virtual void Render() 		 override;

public:
	void CreateMesh();
	void SetMeshResource(const char* path);

private:
	DRAW_INDEX_RESOURCE meshResource;

public:
	std::vector<Vertex> vertices;
	std::vector<UINT>   indices;

public:
	std::shared_ptr<SimpleMaterial> Material = nullptr;

public:
	std::shared_ptr<MatrixPallete> matrixPallete = nullptr;
	std::shared_ptr<BoneWIT> boneWIT = nullptr;

	std::vector<Matrix> offsetMatrices;
	std::vector<BoneComponent*> boneList;
};