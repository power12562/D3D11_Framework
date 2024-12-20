#include "SimpleShadeCube.h"
#include "ShadeTestApp.h"

#include <D3DCore/D3DRenderer.h>
#include <Utility/D3D11Utility.h>
#include <Utility\MemoryUtility.h>

using namespace Utility;

SimpleShadeObject::SimpleShadeObject()
{
    transform = new Transform;
}

SimpleShadeObject::~SimpleShadeObject()
{
    delete transform;
}

SimpleCubeShadeObject::SimpleCubeShadeObject()
{
    Vertex vertices[] =
    {
		{ Vector3(-1.0f, 1.0f, -1.0f),	Vector3(0.0f, 1.0f, 0.0f),  Vector2(1.0f, 0.0f) },
		{ Vector3(1.0f, 1.0f, -1.0f),	Vector3(0.0f, 1.0f, 0.0f),  Vector2(0.0f, 0.0f) },
		{ Vector3(1.0f, 1.0f, 1.0f),	Vector3(0.0f, 1.0f, 0.0f),  Vector2(0.0f, 1.0f) },
		{ Vector3(-1.0f, 1.0f, 1.0f),	Vector3(0.0f, 1.0f, 0.0f),  Vector2(1.0f, 1.0f) },

		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(1.0f, -1.0f, -1.0f),	Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f) },
		{ Vector3(1.0f, -1.0f, 1.0f),	Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, 1.0f),	Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f) },

		{ Vector3(-1.0f, -1.0f, 1.0f),	Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) },
		{ Vector3(-1.0f, 1.0f, -1.0f),	Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) },
		{ Vector3(-1.0f, 1.0f, 1.0f),	Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) },

		{ Vector3(1.0f, -1.0f, 1.0f),	Vector3(1.0f, 0.0f, 0.0f),  Vector2(1.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, -1.0f),	Vector3(1.0f, 0.0f, 0.0f),  Vector2(0.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, -1.0f),	Vector3(1.0f, 0.0f, 0.0f),  Vector2(0.0f, 0.0f) },
		{ Vector3(1.0f, 1.0f, 1.0f),	Vector3(1.0f, 0.0f, 0.0f),  Vector2(1.0f, 0.0f) },

		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.0f) },
		{ Vector3(-1.0f, 1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.0f) },

		{ Vector3(-1.0f, -1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f),  Vector2(1.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f),  Vector2(0.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f),  Vector2(0.0f, 0.0f) },
		{ Vector3(-1.0f, 1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f),  Vector2(1.0f, 0.0f) },
    };

    D3D11_BUFFER_DESC bd{};
    bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices; // 배열 데이터 할당.
    CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &vbData, &m_pVertexBuffer));

    m_VertexBufferStride = sizeof(Vertex);
    m_VertexBufferOffset = 0;

    //인덱스 버퍼용 배열
    UINT indices[] =
    {
       3,1,0, 2,1,3,
       6,4,5, 7,4,6,
       11,9,8, 10,9,11,
       14,12,13, 15,12,14,
       19,17,16, 18,17,19,
       22,20,21, 23,20,22
    };

    // 인덱스 개수 저장.
    m_nIndices = ARRAYSIZE(indices);
    bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    //인덱스 버퍼 생성
    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices;
    CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bd, &ibData, &m_pIndexBuffer));

    //Load Texture
    CheckHRESULT(Utility::CreateTextureFromFile(d3dRenderer.GetDevice(), L"seafloor.dds", nullptr, &m_pTextureRV));

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    CheckHRESULT(d3dRenderer.GetDevice()->CreateSamplerState(&sampDesc, &m_pSamplerLinear));
}

SimpleCubeShadeObject::~SimpleCubeShadeObject()
{
    SafeRelease(m_pIndexBuffer);
    SafeRelease(m_pVertexBuffer);
    SafeRelease(m_pTextureRV);
    SafeRelease(m_pSamplerLinear);
}

void SimpleCubeShadeObject::Update()
{
    transform->UpdateTransform();
}

void SimpleCubeShadeObject::Render(ID3D11Buffer* pConstantBuffer, ID3D11InputLayout* pInputLayout, ID3D11VertexShader* pVertexShader, ID3D11PixelShader* pPixelShader)
{
    using namespace constBuffer;

    auto pDeviceContext = d3dRenderer.GetDeviceContext();

    // 상수버퍼 업데이트
    ConstantBuffer cb;
    cb.mWorld = XMMatrixTranspose(transform->GetWM());
    cb.mView = XMMatrixTranspose(SimpleCubeShadeObject::mView);
    cb.mProjection = XMMatrixTranspose(SimpleCubeShadeObject::mProjection);
    cb.WVP = XMMatrixTranspose(transform->GetWM() * SimpleCubeShadeObject::mView * SimpleCubeShadeObject::mProjection);
    Vector4(lightDir[0], lightDir[1], lightDir[2], lightDir[3]).Normalize(cb.vLightDir);
    cb.vLightColor = Vector4(lightColor[0], lightColor[1], lightColor[2], lightColor[3]);
    pDeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);
	
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정점을 이어서 그릴 방식 설정.
    pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &m_VertexBufferOffset);
    pDeviceContext->IASetInputLayout(pInputLayout);
    pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	// INDEX값의 범위

    pDeviceContext->VSSetShader(pVertexShader, nullptr, 0);
    pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

    pDeviceContext->PSSetShader(pPixelShader, nullptr, 0);
    pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
    pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureRV);
    pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

    auto* pRenderTargetView = d3dRenderer.GetBackBufferRTV();
    pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, d3dRenderer.GetDepthStencilView());  //flip 모드를 사용하기 때문에 매 프레임 설정해주어야 한다.

    pDeviceContext->DrawIndexed(m_nIndices, 0, 0);
}

