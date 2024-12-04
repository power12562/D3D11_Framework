#include "SimpleBoneMeshRender.h"
#include <Framework\D3DRenderer.h>
#include <Material\SimpleMaterial.h>
#include <Light\SimpleDirectionalLight.h>
#include <Framework\ResourceManager.h>
#include <Component/BoneComponent.h>

SimpleBoneMeshRender::SimpleBoneMeshRender()
{
	
}

void SimpleBoneMeshRender::Start()
{
	using namespace Utility;

	wchar_t materialName[50]{};
	swprintf_s(materialName, L"%s (%d)", gameObject.Name.c_str(), gameObject.GetInstanceID());
	Material = ResourceManager<SimpleMaterial>::instance().GetResource(materialName);

	int index = constBuffer.CreatePSConstantBuffers<cb_Material>();
	constBuffer.BindUpdateEvent(Material->cb_material);

	index = constBuffer.CreatePSConstantBuffers<cb_Light>();
	constBuffer.BindUpdateEvent(SimpleDirectionalLight::cb_light);

	index = constBuffer.CreateVSConstantBuffers<MatrixPallete>();
	constBuffer.BindUpdateEvent(matrixPallete);

	index = constBuffer.CreateVSConstantBuffers<BoneWIT>();
	constBuffer.BindUpdateEvent(boneWIT);

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	samplerState.resize(1);
	samplerState.SetSamplerState(0, sampDesc);
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
		if (Material && IsVSShader() && IsPSShader())
		{
			RENDERER_DRAW_DESC desc = GetRendererDesc();
			d3dRenderer.DrawIndex(desc, false);
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
