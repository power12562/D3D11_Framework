#include "SimpleMashComponent.h"
#include <Framework/D3DRenderer.h>
#include <Framework/HLSLManager.h>

#include "..\Source\SimpleDirectionalLight.h"

SimpleMashComponent::SimpleMashComponent()
{
}

SimpleMashComponent::~SimpleMashComponent()
{
    using namespace Utility;
    SafeRelease(m_pTextureRV);
    SafeRelease(m_pNormalMap);
    SafeRelease(m_pSpecularMap);
    SafeRelease(m_pSamplerLinear);
    SafeRelease(m_pEmissiveMap);
    SafeRelease(m_pOpacityMap);

    SafeRelease(drawData.pVertexBuffer);
    SafeRelease(drawData.pIndexBuffer);

    hlslManager.ReleaseSharingShader(L"PixelShader.hlsl");
    hlslManager.ReleaseSharingShader(L"VertexShader.hlsl");
}

void SimpleMashComponent::SetMesh()
{
    using namespace Utility;
    if (vertices.empty() || indices.empty())
        return;

    if (drawData.pIndexBuffer)
        SafeRelease(drawData.pIndexBuffer);

    if (drawData.pVertexBuffer)
        SafeRelease(drawData.pVertexBuffer);

    D3D11_BUFFER_DESC bd{};
    bd.ByteWidth = sizeof(Vertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = &vertices[0];
	CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &drawData.pVertexBuffer));

	// 인덱스 개수 저장.
	drawData.indicesCount = indices.size();
	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	//인덱스 버퍼 생성
	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = &indices[0];
	CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &drawData.pIndexBuffer));

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    CheackHRESULT(d3dRenderer.GetDevice()->CreateSamplerState(&sampDesc, &m_pSamplerLinear));
}

void SimpleMashComponent::Start()
{
    hlslManager.CreateSharingShader(L"PixelShader.hlsl", "ps_4_0", &drawData.pPixelShader);
    hlslManager.CreateSharingShader(L"VertexShader.hlsl", "vs_4_0", &drawData.pVertexShader, &drawData.pInputLayout);

    drawData.vertexBufferStride = sizeof(Vertex);
    drawData.vertexBufferOffset = 0;
}

void SimpleMashComponent::FixedUpdate()
{

}

void SimpleMashComponent::Update()
{

}

void SimpleMashComponent::LateUpdate()
{

}

void SimpleMashComponent::Render()
{
    const auto& pDeviceContext = d3dRenderer.GetDeviceContext();
    pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

    SimpleDirectionalLight::cb_localbool.loaclNormalMap = m_pNormalMap;
    SimpleDirectionalLight::cb_localbool.loaclEmissiveMap = m_pEmissiveMap;
    SimpleDirectionalLight::cb_localbool.loaclOpacityMap = m_pOpacityMap;
    SimpleDirectionalLight::cb_localbool.loaclSpecularMap = m_pSpecularMap;
    
    pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureRV);

    pDeviceContext->PSSetShaderResources(1, 1, &m_pNormalMap);

    pDeviceContext->PSSetShaderResources(2, 1, &m_pSpecularMap);

    pDeviceContext->PSSetShaderResources(3, 1, &m_pEmissiveMap);

    pDeviceContext->PSSetShaderResources(4, 1, &m_pOpacityMap );

    SimpleDirectionalLight::cbuffer.UpdateConstBuffer(SimpleDirectionalLight::cb_localbool);

    if (drawData.pInputLayout && drawData.pVertexBuffer && drawData.pIndexBuffer && drawData.pVertexShader && drawData.pPixelShader)
    {
        d3dRenderer.DrawIndex(drawData, &SimpleDirectionalLight::cbuffer);
    }
}
