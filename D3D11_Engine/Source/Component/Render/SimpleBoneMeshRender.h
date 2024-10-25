#pragma once
#include <Component/Base/RenderComponent.h>

class SimpleMaterial;
class SimpleBoneMeshRender : RenderComponent
{
public:
	struct Vertex
	{
		Vector4 position{ 0,0,0,1 };
		Vector3 normal;
		Vector3 biTangent;
		Vector2 Tex;
		int BlendIndecses[4] = {};
		float BlendWeights[4] = {};
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

private:
	DRAW_INDEX_DATA meshData;

public:
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;

public:
	SimpleMaterial* Material = nullptr;
};