#include "D3DRenderer.h"
#include <Component\Camera.h>
#include <Framework\WinGameApp.h>
#include <Utility\D3D11Utility.h>
#include <Utility\MemoryUtility.h>
#include <_Debug\Console.h>
#include <cassert>

D3DRenderer& d3dRenderer = D3DRenderer::GetInstance();

using namespace Utility;

D3DRenderer::D3DRenderer()
{
    pDevice = nullptr;
    pDeviceContext = nullptr;
    pSwapChain = nullptr;
    pRenderTargetView = nullptr;
    pDepthStencilView = nullptr;
}

D3DRenderer::~D3DRenderer()
{
   
}

void D3DRenderer::Init()
{
    using namespace Utility;

    DXGI_SWAP_CHAIN_DESC swapDesc{}; //����ü�� �Ӽ� ����ü  
    swapDesc.BufferCount = 2; //���� ����
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //���� ��� ��� ����
    swapDesc.OutputWindow = WinGameApp::GetHWND(); //�ڵ� ������
    swapDesc.Windowed = true; //â��� ����
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //��� ���� ����.
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    //���� ������ ����
    SIZE size = WinGameApp::GetClientSize();
    swapDesc.BufferDesc.Width = size.cx;
    swapDesc.BufferDesc.Height = size.cy;

    //ȭ�� �ֻ���
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;

    //���ø� ���� *(MSAA)
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;

    UINT creationFlags = 0;
#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    try
    {
        SIZE clientSize = WinGameApp::GetClientSize();

        //����̽�, ����ü��, ����̽� ���ؽ�Ʈ ����
        CheackHRESULT(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
            D3D11_SDK_VERSION, &swapDesc, &pSwapChain, &pDevice, NULL, &pDeviceContext));

        //����Ÿ�ٺ� ����. (���۹� �̿�)
        ID3D11Texture2D* pBackBufferTexture = nullptr; //�����
        CheackHRESULT(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)); //����ü�� ����۸� �����´�.
        CheackHRESULT(pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &pRenderTargetView)); //�����۸� �����ϴ� �� ����(���� ī��Ʈ ����.)
        SafeRelease(pBackBufferTexture);

        //����Ʈ ����
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = (float)clientSize.cx;
        viewport.Height = (float)clientSize.cy;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        //����Ʈ �Ҵ�
        pDeviceContext->RSSetViewports(1, &viewport);

        //���� ���ٽ� ����
        D3D11_TEXTURE2D_DESC descDepth = {};
        descDepth.Width = clientSize.cx;
        descDepth.Height = clientSize.cy;
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepth.SampleDesc.Count = 1;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;

        ID3D11Texture2D* textureDepthStencil = nullptr;
        CheackHRESULT(pDevice->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil));

        // Create the depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
        descDSV.Format = descDepth.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        CheackHRESULT(pDevice->CreateDepthStencilView(textureDepthStencil, &descDSV, &pDepthStencilView));
        SafeRelease(textureDepthStencil);

        pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

        CreateVSPSConstantBuffers<cbuffer_Transform>();
        CreateVSPSConstantBuffers<cbuffer_Camera>();
    }
    catch (const std::exception& ex)
    {
        Debug_printf("%s\n", ex.what());
        assert(!"D3D Init Error");
        return;
    }
    return;
}

void D3DRenderer::Uninit()
{
    for (auto& cbuffer : vs_cbufferList)
    {
        SafeRelease(cbuffer);
    }
    for (auto& cbuffer : ps_cbufferList)
    {
        SafeRelease(cbuffer);
    }

    SafeRelease(pRenderTargetView);
    SafeRelease(pDepthStencilView);
    SafeRelease(pSwapChain);
    SafeRelease(pDeviceContext);
    if (SafeRelease(pDevice) != 0)
    {
        __debugbreak(); //Device refcounter err.
    }
}

D3DRenderer::REG_INDEX D3DRenderer::CreateVSPSConstantBuffers(const char* key, unsigned int buffer_size)
{
    REG_INDEX index{};
    index.vs_index = CreateVSConstantBuffers(key, buffer_size);
    index.ps_index = CreatePSConstantBuffers(key, buffer_size);

    return index;
}

int D3DRenderer::CreateVSConstantBuffers(const char* key, unsigned int buffer_size)
{
    assert((buffer_size % 16) == 0 && "Constant Buffer size must be 16 - byte aligned");

    if (vs_cbufferList.size() == D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
    {
        __debugbreak(); //��� ���� �ִ� ���� ����.
        return -1;
    }

    if (vs_cbufferMap.find(key) != vs_cbufferMap.end())
    {
        __debugbreak(); //�̹� �����ϴ� Ű��.
        return -1;
    }

    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // ��� ���۴� dynamic���� �����ϰ� ���� ����.
    bufferDesc.ByteWidth = buffer_size;  // ��� ���� ũ��
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    int regIndex = (int)vs_cbufferList.size();
    vs_cbufferMap[key] = regIndex;

    ID3D11Buffer* cBufferTemp{};
    Utility::CheackHRESULT(pDevice->CreateBuffer(&bufferDesc, nullptr, &cBufferTemp));
    vs_cbufferList.push_back(cBufferTemp);

    return regIndex;
}

int D3DRenderer::CreatePSConstantBuffers(const char* key, unsigned int buffer_size)
{
    assert((buffer_size % 16) == 0 && "Constant Buffer size must be 16 - byte aligned");

    if (ps_cbufferList.size() == D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
    {
        __debugbreak(); //��� ���� �ִ� ���� ����.
        return -1;
    }

    if (ps_cbufferMap.find(key) != ps_cbufferMap.end())
    {
        __debugbreak(); //�̹� �����ϴ� Ű��.
        return -1;
    }

    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // ��� ���۴� dynamic���� �����ϰ� ���� ����.
    bufferDesc.ByteWidth = buffer_size;  // ��� ���� ũ��
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    int regIndex = (int)ps_cbufferList.size();
    ps_cbufferMap[key] = regIndex;

    ID3D11Buffer* cBufferTemp{};
    Utility::CheackHRESULT(pDevice->CreateBuffer(&bufferDesc, nullptr, &cBufferTemp));
    ps_cbufferList.push_back(cBufferTemp);

    return regIndex;
}

void D3DRenderer::BegineDraw()
{   
    pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);  //flip ��带 ����ϱ� ������ �� ������ �������־�� �Ѵ�.
    pDeviceContext->ClearRenderTargetView(pRenderTargetView, backgroundColor);  // ȭ�� ĥ�ϱ�.  
    pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);  //���� ���� �ʱ�ȭ
    pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, d3dRenderer.GetDepthStencilView());  //flip ��带 ����ϱ� ������ �� ������ �������־�� �Ѵ�.

    cbuffer_Camera cb;
    cb.Projection = Camera::GetMainCamera()->GetPM();
    cb.View = Camera::GetMainCamera()->GetVM();

    UpdateConstBuffer(cb); //ī�޶� ���� ���ε�
}

void D3DRenderer::EndDraw()
{
    pSwapChain->Present(0, 0);     // Present the information rendered to the back buffer to the front buffer (the screen)
}

void D3DRenderer::DrawIndex(DRAW_INDEX_DATA& data)
{
    SetConstBuffer(); // ��� ���� ���ε�.

    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ������ �̾ �׸� ��� ����.
    pDeviceContext->IASetVertexBuffers(0, 1, &data.pVertexBuffer, &data.vertexBufferStride, &data.vertexBufferOffset);
    pDeviceContext->IASetInputLayout(data.pInputLayout);
    pDeviceContext->IASetIndexBuffer(data.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	// INDEX���� ����

    pDeviceContext->VSSetShader(data.pVertexShader, nullptr, 0);

    pDeviceContext->PSSetShader(data.pPixelShader, nullptr, 0);

    pDeviceContext->DrawIndexed(data.indicesCount, 0, 0);
}

void D3DRenderer::SetConstBuffer()
{
    for (int i = 0; i < vs_cbufferList.size(); i++)
        pDeviceContext->VSSetConstantBuffers(i, 1, &vs_cbufferList[i]);

    for (int i = 0; i < ps_cbufferList.size(); i++)
        pDeviceContext->PSSetConstantBuffers(i, 1, &ps_cbufferList[i]);
}
