#include "SimpleBoneMeshRender.h"
#include <Framework\D3DRenderer.h>
#include <Material\SimpleMaterial.h>
#include <Light\SimpleDirectionalLight.h>
#include <Framework\ResourceManager.h>
#include <Component/BoneComponent.h>

SimpleBoneMeshRender::SimpleBoneMeshRender()
{
	if (Material == nullptr)
	{
		Material = ResourceManager<SimpleMaterial>::instance().GetResource(L"Standard");
	}
}

SimpleBoneMeshRender::~SimpleBoneMeshRender()
{
	using namespace Utility;
	SafeRelease(meshResource->pVertexBuffer);
	SafeRelease(meshResource->pIndexBuffer);
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
	if (boneWIT && matrixPallete && meshResource->pVertexBuffer && meshResource->pIndexBuffer)
	{
		for (int i = 0; i < boneList.size(); i++)
		{
			matrixPallete->MatrixPalleteArray[i] = XMMatrixTranspose(offsetMatrices[i] * boneList[i]->GetBoneMatrix());

			Matrix Inverse = XMMatrixInverse(nullptr, offsetMatrices[i] * boneList[i]->GetBoneMatrix());
			Inverse = Utility::XMMatrixIsNaN(Inverse) ? Matrix() : Inverse;
			boneWIT->BoneWIT[i] = Inverse;
		}
		const auto& pDeviceContext = d3dRenderer.GetDeviceContext();
		if (Material)
		{
			Material->cbuffer.UpdateEvent();
			d3dRenderer.DrawIndex(*meshResource, *Material);
		}
	}
	else
	{
		__debugbreak(); //데이터 없음.
	}
}

void SimpleBoneMeshRender::CreateMesh()
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

void SimpleBoneMeshRender::SetMeshResource(const char* path)
{
	if (meshResource == nullptr)
	{
		meshResource = std::make_shared<DRAW_INDEX_DATA>();
	}
}