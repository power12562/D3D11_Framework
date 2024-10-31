#include "SimpleMeshRender.h"
#include <Framework\D3DRenderer.h>
#include <Material\SimpleMaterial.h>
#include <Light\SimpleDirectionalLight.h>
#include <Framework\ResourceManager.h>

SimpleMeshRender::SimpleMeshRender()
{
	if (Material == nullptr)
	{
		Material = ResourceManager<SimpleMaterial>::instance().GetResource(L"Standard");
	}
}

SimpleMeshRender::~SimpleMeshRender()
{
	using namespace Utility;
	SafeRelease(meshResource->pVertexBuffer);
	SafeRelease(meshResource->pIndexBuffer);
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
	if (!meshResource->pVertexBuffer || !meshResource->pIndexBuffer)
	{
		__debugbreak(); //데이터 없음.
		return;
	}
	
    const auto& pDeviceContext = d3dRenderer.GetDeviceContext();
    if (Material)
    {
		Material->cbuffer.UpdateEvent();
        d3dRenderer.DrawIndex(*meshResource, *Material);
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

	vertices.clear();
	indices.clear();
}

void SimpleMeshRender::SetMeshResource(const char* path)
{
	if (meshResource == nullptr)
	{
		meshResource = std::make_shared<DRAW_INDEX_DATA>();
	}
}
