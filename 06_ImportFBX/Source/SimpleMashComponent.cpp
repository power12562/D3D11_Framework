#include "SimpleMashComponent.h"
#include <Framework/D3DRenderer.h>
#include <Framework/HLSLManager.h>

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
}

void SimpleMashComponent::Start()
{
    hlslManager.CreateSharingShader(L"PixelShader.hlsl", "ps_4_0", &drawData.pPixelShader);
    hlslManager.CreateSharingShader(L"VertexShader.hlsl", "vs_4_0", &drawData.pVertexShader);

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    auto pDevice = d3dRenderer.GetDevice();
    ID3D10Blob* vertexShaderBuffer = nullptr;
    Utility::CheackHRESULT(Utility::CompileShaderFromFile(L"VertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));
    Utility::CheackHRESULT(pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
        vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &drawData.pInputLayout));
    vertexShaderBuffer->Release();

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
    if(drawData.pInputLayout && drawData.pVertexBuffer && drawData.pIndexBuffer && drawData.pVertexShader && drawData.pPixelShader)
        d3dRenderer.DrawIndex(drawData);
}
