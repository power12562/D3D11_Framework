#include "SimpleMeshRender.h"
#include <D3DCore/D3DRenderer.h>
#include <Material\BlingPhongMaterial.h>
#include <Light\SimpleDirectionalLight.h>
#include <Manager/ResourceManager.h>
#include <Math/Mathf.h>

SimpleMeshRender::SimpleMeshRender()
{
	isSkinning = false;
}

void SimpleMeshRender::Start()
{
	using namespace Utility;

	// Create the sample state
	samplerState.resize(2);
	D3D11_SAMPLER_DESC SamplerDesc = {};
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerState.SetSamplerState(0, SamplerDesc);

	//Shadow Sampler
	SamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerState.SetSamplerState(1, SamplerDesc);
}

void SimpleMeshRender::FixedUpdate()
{
}

void SimpleMeshRender::Update()
{
}

void SimpleMeshRender::LateUpdate()
{
}

void SimpleMeshRender::Render()
{
	if (!meshResource->pVertexBuffer || !meshResource->pIndexBuffer)
	{
		__debugbreak(); //데이터 없음.
		return;
	}
	
    const auto& pDeviceContext = d3dRenderer.GetDeviceContext();
    if (IsVSShader() && IsPSShader())
    {
		RENDERER_DRAW_DESC desc = GetRendererDesc();
        d3dRenderer.DrawIndex(desc);
    } 
}

void SimpleMeshRender::CreateMesh()
{
	using namespace Utility;
	if (vertices.empty() || indices.empty())
		return;

	if (meshResource->pIndexBuffer)
		SafeRelease(meshResource->pIndexBuffer);

	if (meshResource->pVertexBuffer)
		SafeRelease(meshResource->pVertexBuffer);

	meshResource->vertexBufferOffset = 0;
	meshResource->vertexBufferStride = sizeof(Vertex);
	D3D11_BUFFER_DESC bd{};
	bd.ByteWidth = sizeof(Vertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices.data();
	CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &meshResource->pVertexBuffer));
	
	meshResource->indicesCount = indices.size();
	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices.data();
	CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &meshResource->pIndexBuffer));

	//Create bounding box
	BoundingBox box;
	box.CreateFromPoints(box, vertices.size(), reinterpret_cast<XMFLOAT3*>(vertices.data()), sizeof(Vertex));
	if (gameObject.Bounds.Center.x < Mathf::FLOAT_MAX)
	{
		BoundingBox::CreateMerged(gameObject.Bounds, gameObject.Bounds, box);
	}
	else
		gameObject.Bounds = box;
	
	if (Transform* root = gameObject.transform.RootParent)
	{
		box.Transform(box, transform.scale.x, transform.rotation, transform.position);
		if (root->gameObject.Bounds.Center.x < Mathf::FLOAT_MAX)
		{
			BoundingBox::CreateMerged(root->gameObject.Bounds, root->gameObject.Bounds, box);
		}
		else
			root->gameObject.Bounds = box;
	}

	vertices.clear();
	indices.clear();
}
