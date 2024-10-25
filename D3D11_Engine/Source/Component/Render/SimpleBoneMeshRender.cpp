#include "SimpleBoneMeshRender.h"

#include <Framework\D3DRenderer.h>
#include <Material\SimpleMaterial.h>
#include <Light\SimpleDirectionalLight.h>
#include <Framework\MaterialManager.h>
#include <Component/BoneComponent.h>

SimpleBoneMeshRender::SimpleBoneMeshRender()
{
	if (Material == nullptr)
	{
		Material = materialManager.GetMaterial(L"Standard");
	}
}

SimpleBoneMeshRender::~SimpleBoneMeshRender()
{
	using namespace Utility;
	SafeRelease(meshData.pVertexBuffer);
	SafeRelease(meshData.pIndexBuffer);
}

void SimpleBoneMeshRender::Start()
{

}

void SimpleBoneMeshRender::FixedUpdate()
{
}

void SimpleBoneMeshRender::Update()
{

}

void SimpleBoneMeshRender::LateUpdate()
{
}

void SimpleBoneMeshRender::Render()
{
	if (!meshData.pVertexBuffer || !meshData.pIndexBuffer)
	{
		__debugbreak(); //데이터 없음.
		return;
	}
	
	const auto& pDeviceContext = d3dRenderer.GetDeviceContext();
	if (Material)
	{
		Material->cbuffer.UpdateEvent();
		d3dRenderer.DrawIndex(meshData, *Material);
	}
}

void SimpleBoneMeshRender::CreateMesh()
{
	using namespace Utility;
	if (vertices.empty() || indices.empty())
		return;

	if (meshData.pIndexBuffer)
		SafeRelease(meshData.pIndexBuffer);

	if (meshData.pVertexBuffer)
		SafeRelease(meshData.pVertexBuffer);

	meshData.vertexBufferOffset = 0;
	meshData.vertexBufferStride = sizeof(Vertex);
	D3D11_BUFFER_DESC bd{};
	bd.ByteWidth = sizeof(Vertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices.data();
	CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &meshData.pVertexBuffer));

	meshData.indicesCount = indices.size();
	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices.data();
	CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &meshData.pIndexBuffer));
}
