#pragma once
#include <Component/Base/RenderComponent.h>
#include <Framework/D3DConstBuffer.h>
#include <Framework/D3DTexture2D.h>
#include <Framework/D3DSamplerState.h>

class MeshRender : public RenderComponent
{
public:
	MeshRender();
	virtual ~MeshRender() override;

public:
	virtual void CreateMesh() = 0;
protected:
	DRAW_INDEX_RESOURCE meshResource;

public:
	virtual void Start() = 0;
protected:
	virtual void FixedUpdate() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render() = 0;

public:
	void SetMeshResource(const wchar_t* path);
	int MeshID = -1;
public:
	D3DConstBuffer constBuffer;
	D3DTexture2D texture2D;
	D3DSamplerState samplerState;
public:
	void SetVertexShader(const wchar_t* path);
	void SetPixelShader(const wchar_t* path);
private:
	ID3D11InputLayout* pInputLayout = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader* pPixelShader = nullptr;
};