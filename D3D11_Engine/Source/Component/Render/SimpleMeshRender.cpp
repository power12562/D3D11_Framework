#include "SimpleMeshRender.h"
#include <Framework\D3DRenderer.h>
#include <Material\SimpleMaterial.h>

SimpleMeshRender::SimpleMeshRender()
{
}

SimpleMeshRender::~SimpleMeshRender()
{
	using namespace Utility;
	SafeRelease(meshData.pVertexBuffer);
	SafeRelease(meshData.pIndexBuffer);
}

void SimpleMeshRender::Start()
{
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
    const auto& pDeviceContext = d3dRenderer.GetDeviceContext();
    if (Material)
    {
        Material->cbuffer.UpdateConstBuffer(Material->Light);
        Material->cbuffer.UpdateConstBuffer(Material->Material);
        d3dRenderer.DrawIndex(meshData, *Material);
    } 
}

void SimpleMeshRender::CreateMesh()
{
	using namespace Utility;
	if (vertices.empty() || indices.empty())
		return;

	if (meshData.pIndexBuffer)
		SafeRelease(meshData.pIndexBuffer);

	if (meshData.pVertexBuffer)
		SafeRelease(meshData.pVertexBuffer);

	D3D11_BUFFER_DESC bd{};
	bd.ByteWidth = sizeof(Vertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices.data();
	CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &meshData.pVertexBuffer));

	meshData.indicesCount = indices.size();
	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices.data();
	CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &meshData.pIndexBuffer));
}
