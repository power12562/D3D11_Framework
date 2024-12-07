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
	DirectX::SimpleMath::Color baseColor{1,1,1,1};

public:
	bool isAlpha = false;

public:
	bool IsVSShader() { return pInputLayout && pVertexShader; }
	bool IsPSShader() { return !!pPixelShader; }

public:
	void CopyShader(MeshRender& rhs);
public:
	void SetVertexShader(const wchar_t* path);
	void SetPixelShader(const wchar_t* path);
public:
	void ResetVertexShader();
	void ResetPixelShader();
private:
	ID3D11InputLayout* pInputLayout = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader* pPixelShader = nullptr;

public:
	void SetFillMode(D3D11_FILL_MODE mode);
	void SetCullMode(D3D11_CULL_MODE mode);
	void SetRRState(D3D11_RASTERIZER_DESC& desc);
	void ResetRRState();

private:
	D3D11_RASTERIZER_DESC currRRdesc;
	ID3D11RasterizerState* pRRState = nullptr;
public:
	RENDERER_DRAW_DESC GetRendererDesc();

};