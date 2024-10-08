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

    DXGI_SWAP_CHAIN_DESC swapDesc{}; //스왑체인 속성 구조체  
    swapDesc.BufferCount = 2; //버퍼 개수
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //버퍼 사용 방식 지정
    swapDesc.OutputWindow = WinGameApp::GetHWND(); //핸들 윈도우
    swapDesc.Windowed = true; //창모드 유무
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //출력 포멧 지정.
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    //버퍼 사이즈 지정
    SIZE size = WinGameApp::GetClientSize();
    swapDesc.BufferDesc.Width = size.cx;
    swapDesc.BufferDesc.Height = size.cy;

    //화면 주사율
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;

    //샘플링 설정 *(MSAA)
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;

    UINT creationFlags = 0;
#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    try
    {
        SIZE clientSize = WinGameApp::GetClientSize();

        //디바이스, 스왑체인, 디바이스 컨텍스트 생성
        CheackHRESULT(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
            D3D11_SDK_VERSION, &swapDesc, &pSwapChain, &pDevice, NULL, &pDeviceContext));

        //렌더타겟뷰 생성. (백퍼버 이용)
        ID3D11Texture2D* pBackBufferTexture = nullptr; //백버퍼
        CheackHRESULT(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)); //스왑체인 백버퍼를 가져온다.
        CheackHRESULT(pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &pRenderTargetView)); //백퍼퍼를 참조하는 뷰 생성(참조 카운트 증가.)
        SafeRelease(pBackBufferTexture);

        //뷰포트 설정
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = (float)clientSize.cx;
        viewport.Height = (float)clientSize.cy;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        //뷰포트 할당
        pDeviceContext->RSSetViewports(1, &viewport);

        //깊이 스텐실 버퍼
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
        __debugbreak(); //상수 버퍼 최대 개수 도달.
        return -1;
    }

    if (vs_cbufferMap.find(key) != vs_cbufferMap.end())
    {
        __debugbreak(); //이미 존재하는 키값.
        return -1;
    }

    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // 상수 버퍼는 dynamic으로 생성하고 쓰기 전용.
    bufferDesc.ByteWidth = buffer_size;  // 상수 버퍼 크기
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
        __debugbreak(); //상수 버퍼 최대 개수 도달.
        return -1;
    }

    if (ps_cbufferMap.find(key) != ps_cbufferMap.end())
    {
        __debugbreak(); //이미 존재하는 키값.
        return -1;
    }

    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // 상수 버퍼는 dynamic으로 생성하고 쓰기 전용.
    bufferDesc.ByteWidth = buffer_size;  // 상수 버퍼 크기
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
    pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);  //flip 모드를 사용하기 때문에 매 프레임 설정해주어야 한다.
    pDeviceContext->ClearRenderTargetView(pRenderTargetView, backgroundColor);  // 화면 칠하기.  
    pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);  //깊이 버퍼 초기화
    pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, d3dRenderer.GetDepthStencilView());  //flip 모드를 사용하기 때문에 매 프레임 설정해주어야 한다.

    cbuffer_Camera cb;
    cb.Projection = Camera::GetMainCamera()->GetPM();
    cb.View = Camera::GetMainCamera()->GetVM();

    UpdateConstBuffer(cb); //카메라 버퍼 바인딩
}

void D3DRenderer::EndDraw()
{
    pSwapChain->Present(0, 0);     // Present the information rendered to the back buffer to the front buffer (the screen)
}

void D3DRenderer::DrawIndex(DRAW_INDEX_DATA& data)
{
    SetConstBuffer(); // 상수 버퍼 바인딩.

    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정점을 이어서 그릴 방식 설정.
    pDeviceContext->IASetVertexBuffers(0, 1, &data.pVertexBuffer, &data.vertexBufferStride, &data.vertexBufferOffset);
    pDeviceContext->IASetInputLayout(data.pInputLayout);
    pDeviceContext->IASetIndexBuffer(data.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	// INDEX값의 범위

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
