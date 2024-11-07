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
	if (meshResource.use_count() == 1)
	{
		SafeRelease(meshResource->pVertexBuffer);
		SafeRelease(meshResource->pIndexBuffer);
	}
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
	if (meshResource->pVertexBuffer && meshResource->pIndexBuffer)
	{
		Matrix temp{};
		for (int i = 0; i < boneList.size(); i++)
		{
			temp = offsetMatrices->data[i] * boneList[i]->GetBoneMatrix();

			matrixPallete.MatrixPalleteArray[i] = XMMatrixTranspose(temp);

			temp = XMMatrixInverse(nullptr, temp);
			temp = Utility::XMMatrixIsNaN(temp) ? Matrix() : temp;
			boneWIT.BoneWIT[i] = temp;
		}
		const auto& pDeviceContext = d3dRenderer.GetDeviceContext();
		if (Material && Material->IsShader())
		{
			d3dRenderer.DrawIndex(*meshResource, *Material, gameObject.transform);
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

void SimpleBoneMeshRender::SetMeshResource(const wchar_t* path)
{
	if (MeshID < 0)
	{
		__debugbreak();
		return;
	}

	using namespace utfConvert;
	if (meshResource == nullptr)
	{
		//고유의 fbx 경로 + index
		meshResource = GetResourceManager<DRAW_INDEX_DATA>().GetResource(path, MeshID);
	}
}