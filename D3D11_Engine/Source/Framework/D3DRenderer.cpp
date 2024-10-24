#include "D3DRenderer.h"
#include <Component\Camera\Camera.h>
#include <Framework\WinGameApp.h>
#include <Framework/D3DConstBuffer.h>
#include <Material\SimpleMaterial.h>
#include <Utility\D3D11Utility.h>
#include <Utility\MemoryUtility.h>
#include <Framework/TextureManager.h>
#include <_Debug\Console.h>
#include <cassert>

D3DRenderer& d3dRenderer = D3DRenderer::GetInstance();

using namespace Utility;

namespace cbuffer
{
    cb_Camera camera;
}

D3DRenderer::D3DRenderer()
{
    pDevice = nullptr;
    pDeviceContext = nullptr;
    pSwapChain = nullptr;
    pRenderTargetView = nullptr;
    pDepthStencilView = nullptr;
    pBlendState = nullptr;
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
        CheckHRESULT(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
            D3D11_SDK_VERSION, &swapDesc, &pSwapChain, &pDevice, NULL, &pDeviceContext));

        //렌더타겟뷰 생성. (백퍼버 이용)
        ID3D11Texture2D* pBackBufferTexture = nullptr; //백버퍼
        CheckHRESULT(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)); //스왑체인 백버퍼를 가져온다.
        CheckHRESULT(pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &pRenderTargetView)); //백퍼퍼를 참조하는 뷰 생성(참조 카운트 증가.)
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
        CheckHRESULT(pDevice->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil));

        // Create the depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
        descDSV.Format = descDepth.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        CheckHRESULT(pDevice->CreateDepthStencilView(textureDepthStencil, &descDSV, &pDepthStencilView));
        SafeRelease(textureDepthStencil);

        //Set alpha blend
        D3D11_BLEND_DESC blendDesc;
        ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

        // 블렌딩 상태 설정
        blendDesc.AlphaToCoverageEnable = FALSE; // Alpha To Coverage 비활성화
        blendDesc.IndependentBlendEnable = FALSE; // 독립 블렌드 비활성화
        blendDesc.RenderTarget[0].BlendEnable = TRUE; // 블렌딩 활성화
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // 소스 블렌드 (소스 알파 사용)
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // 대상 블렌드 (1 - 소스 알파)
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // 블렌드 연산
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // 알파 소스 블렌드
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // 알파 대상 블렌드
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // 알파 블렌드 연산
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 모든 색상 채널 쓰기 허용
        CheckHRESULT(pDevice->CreateBlendState(&blendDesc, &pBlendState));

        pDeviceContext->OMSetBlendState(pBlendState, nullptr, 0xFFFFFFFF);

        D3DConstBuffer::CreateStaticCbuffer();
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
    D3DConstBuffer::ReleaseStaticCbuffer();

    SafeRelease(pBlendState);
    SafeRelease(pRenderTargetView);
    SafeRelease(pDepthStencilView);
    SafeRelease(pSwapChain);
    SafeRelease(pDeviceContext);

    textureManager.Get1x1Texture()->Release();

    ULONG refcount = SafeRelease(pDevice);
    if (refcount != 0)
    {
        __debugbreak(); //Device refcounter err.
    }
}

void D3DRenderer::BegineDraw()
{   
    pDeviceContext->ClearRenderTargetView(pRenderTargetView, backgroundColor);  // 화면 칠하기.  
    pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);  //깊이 버퍼 초기화
    pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);  //flip 모드를 사용하기 때문에 매 프레임 설정해주어야 한다.

    Camera* mainCam = Camera::GetMainCamera();
    cbuffer::camera.MainCamPos = mainCam->transform.position;
    cbuffer::camera.Projection = mainCam->GetPM();
    cbuffer::camera.View = mainCam->GetVM();
    D3DConstBuffer::UpdateStaticCbuffer(cbuffer::camera);
}

void D3DRenderer::EndDraw()
{
    pSwapChain->Present(0, 0);     // Present the information rendered to the back buffer to the front buffer (the screen)
}

void D3DRenderer::DrawIndex(DRAW_INDEX_DATA& data, SimpleMaterial& material)
{
    material.cbuffer.SetConstBuffer();

    auto noneTexture = textureManager.Get1x1Texture();
    for (int i = 0; i < (E_TEXTURE_INDEX::Null); i++)
    {       
        if (i != E_TEXTURE_INDEX::Emissive)
        {
            if (material.textures[i])
                pDeviceContext->PSSetShaderResources(i, 1, &material.textures[i]);
            else
                pDeviceContext->PSSetShaderResources(i, 1, &noneTexture);
        }
        else
        {
            pDeviceContext->PSSetShaderResources(i, 1, &material.textures[i]);
        }
    }
    pDeviceContext->PSSetSamplers(0, 1, &material.pSamplerLinear);

    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정점을 이어서 그릴 방식 설정.
    pDeviceContext->IASetVertexBuffers(0, 1, &data.pVertexBuffer, &data.vertexBufferStride, &data.vertexBufferOffset);
    pDeviceContext->IASetInputLayout(material.pInputLayout);
    pDeviceContext->IASetIndexBuffer(data.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	// INDEX값의 범위

    pDeviceContext->VSSetShader(material.pVertexShader, nullptr, 0);

    pDeviceContext->PSSetShader(material.pPixelShader, nullptr, 0);

    pDeviceContext->DrawIndexed(data.indicesCount, 0, 0);
}
