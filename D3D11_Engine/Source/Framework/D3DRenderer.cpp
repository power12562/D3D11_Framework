#include "D3DRenderer.h"
#include <Component\Camera\Camera.h>
#include <Framework\WinGameApp.h>
#include <Framework/D3DConstBuffer.h>
#include <Framework/Transform.h>
#include <Material\BlingPhongMaterial.h>
#include <Utility\D3D11Utility.h>
#include <Utility\MemoryUtility.h>
#include <Framework/TextureManager.h>
#include <Framework/SceneManager.h>
#include <Utility/Console.h>
#include <dxgi1_4.h>
#include <cassert>
#include <Psapi.h>
#include <Framework/D3DSamplerState.h>
#include <Framework/D3DTexture2D.h>

D3DRenderer& d3dRenderer = D3DRenderer::GetInstance();

using namespace Utility;

namespace cbuffer
{
    cb_Camera camera;
    cb_Transform transform;
}

D3DRenderer::D3DRenderer()
{
    pDevice = nullptr;
    pDeviceContext = nullptr;
    pSwapChain = nullptr;
    pRenderTargetView = nullptr;
    pDepthStencilView = nullptr;
    pBlendState = nullptr;
    pDefaultRRState = nullptr;
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
        //dxgi 디바이스용(Vram 체크)
        CheckHRESULT(CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&pDXGIFactory));
        CheckHRESULT(pDXGIFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(&pDXGIAdapter)));


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

        //레스터화 기본 규칙
        D3D11_RASTERIZER_DESC rasterDesc;
        ZeroMemory(&rasterDesc, sizeof(rasterDesc));
        rasterDesc.FillMode = D3D11_FILL_SOLID;
        rasterDesc.CullMode = D3D11_CULL_NONE;  // 컬링 없음
        rasterDesc.FrontCounterClockwise = false; // 기본값
        CreateRRState(rasterDesc, &pDefaultRRState);
        SetRRState(pDefaultRRState);

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
        const char* er = ex.what();
        Debug_printf("%s\n", er);
        assert(!"D3D Init Error");
        return;
    }
    return;
}

void D3DRenderer::Uninit()
{
    //Vram용 dxgi 개체
    SafeRelease(pDXGIAdapter);
    SafeRelease(pDXGIFactory);
     
    //dxd11 개체
    D3DConstBuffer::ReleaseStaticCbuffer();
    SafeRelease(pDefaultRRState);
    SafeRelease(pBlendState);
    SafeRelease(pRenderTargetView);
    SafeRelease(pDepthStencilView);
    SafeRelease(pSwapChain);
    SafeRelease(pDeviceContext);

    textureManager.ReleaseDefaultTexture();

#ifdef _DEBUG
    //ID3D11Debug* debug = nullptr;
    //pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug));
    //if (debug)
    //{
    //    debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL); // 상세 정보 출력
    //    debug->Release();
    //}
#endif // _DEBUG
    ULONG refcount = SafeRelease(pDevice);
    if (refcount != 0)
    {
        __debugbreak(); //Device refcounter err.
    }
}

void D3DRenderer::reserveRenderQueue(size_t size)
{
    opaquerenderOueue.clear();
    opaquerenderOueue.reserve(size);

    alphaRenderQueue.clear();
    alphaRenderQueue.reserve(size);
}

void D3DRenderer::SetRRState(ID3D11RasterizerState* rasterState)
{
    if (rasterState)
    {
        pDeviceContext->RSSetState(rasterState);
    }
    else
    {
        pDeviceContext->RSSetState(pDefaultRRState);
    }
}

void D3DRenderer::CreateRRState(D3D11_RASTERIZER_DESC& RASTERIZER_DESC, ID3D11RasterizerState** rasterState)
{
    pDevice->CreateRasterizerState(&RASTERIZER_DESC, rasterState);
}

namespace BytesHelp
{
    constexpr UINT64 toMB = 1024 * 1024;  // 메가바이트 단위로 변환용
}

USAGE_VRAM_INFO D3DRenderer::GetLocalVramUsage()
{
    DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
    pDXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);
    USAGE_VRAM_INFO info{};
    info.Budget = videoMemoryInfo.Budget / BytesHelp::toMB;
    info.Usage = videoMemoryInfo.CurrentUsage / BytesHelp::toMB;
    return info;
}

USAGE_VRAM_INFO D3DRenderer::GetNonLocalVramUsage()
{
    DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
    pDXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL, &videoMemoryInfo);

    USAGE_VRAM_INFO info{};
    info.Budget = videoMemoryInfo.Budget / BytesHelp::toMB;
    info.Usage = videoMemoryInfo.CurrentUsage / BytesHelp::toMB;

    return info;
}

SYSTEM_VRAM_INFO D3DRenderer::GetSystemVramInfo()
{
    DXGI_ADAPTER_DESC desc;
    pDXGIAdapter->GetDesc(&desc);

    SYSTEM_VRAM_INFO info{};
    info.DedicatedVideoMemory = desc.DedicatedVideoMemory / BytesHelp::toMB;
    info.SharedSystemMemory = desc.SharedSystemMemory / BytesHelp::toMB;

    return info;
}

SYSTEM_MEMORY_INFO D3DRenderer::GetSystemMemoryInfo()
{
    HANDLE hProcess = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS_EX pmc{};
    pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
    GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));          

    SYSTEM_MEMORY_INFO info{}; 
    info.PagefileUsage = pmc.PagefileUsage / BytesHelp::toMB;
    info.PrivateUsage = pmc.PrivateUsage / BytesHelp::toMB;
    info.WorkingSetSize = pmc.WorkingSetSize / BytesHelp::toMB;     

    return info;
}

void D3DRenderer::BegineDraw()
{   
    pDeviceContext->ClearRenderTargetView(pRenderTargetView, backgroundColor);  // 화면 칠하기.  
    pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);  //깊이 버퍼 초기화
    pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);  //flip 모드를 사용하기 때문에 매 프레임 설정해주어야 한다.

	Camera* mainCam = Camera::GetMainCamera();
	cbuffer::camera.MainCamPos = mainCam->transform.position;
	cbuffer::camera.View = XMMatrixTranspose(mainCam->GetVM());
	cbuffer::camera.Projection = XMMatrixTranspose(mainCam->GetPM());
	D3DConstBuffer::UpdateStaticCbuffer(cbuffer::camera);

    size_t objCounts = sceneManager.GetObjectsCount();
    if (opaquerenderOueue.capacity() < objCounts)
    {     
        opaquerenderOueue.reserve(objCounts);
    }
    if (alphaRenderQueue.capacity() < objCounts)
    {     
        alphaRenderQueue.reserve(objCounts);
    }
}

void D3DRenderer::DrawIndex(RENDERER_DRAW_DESC& darwDesc, bool isAlpha)
{
    if (isAlpha)
        alphaRenderQueue.emplace_back(darwDesc);
    else
        opaquerenderOueue.emplace_back(darwDesc);
}

void D3DRenderer::EndDraw()
{
    for (auto& item : opaquerenderOueue)
    {
        cbuffer::transform.World = XMMatrixTranspose(item.pTransform->GetWM());
        cbuffer::transform.WorldInverseTranspose = XMMatrixInverse(nullptr, item.pTransform->GetWM());
        cbuffer::transform.WVP = XMMatrixTranspose(item.pTransform->GetWM() * Camera::GetMainCamera()->GetVM() * Camera::GetMainCamera()->GetPM());
        D3DConstBuffer::UpdateStaticCbuffer(cbuffer::transform);

        Draw(item);
    }
    for (auto& item : alphaRenderQueue)
    {
        cbuffer::transform.World = XMMatrixTranspose(item.pTransform->GetWM());
        cbuffer::transform.WorldInverseTranspose = XMMatrixInverse(nullptr, item.pTransform->GetWM());
        cbuffer::transform.WVP = XMMatrixTranspose(item.pTransform->GetWM() * Camera::GetMainCamera()->GetVM() * Camera::GetMainCamera()->GetPM());
        D3DConstBuffer::UpdateStaticCbuffer(cbuffer::transform);

        Draw(item);
    }
    opaquerenderOueue.clear();
    alphaRenderQueue.clear();
}

void D3DRenderer::Draw(RENDERER_DRAW_DESC& drawDesc)
{
    drawDesc.pConstBuffer->UpdateEvent();
    drawDesc.pConstBuffer->SetConstBuffer();
    for (int i = 0; i < drawDesc.pSamperState->size(); i++)
    {
        ID3D11SamplerState* sampler = (*drawDesc.pSamperState)[i];
        pDeviceContext->VSSetSamplers(i, 1, &sampler);
        pDeviceContext->PSSetSamplers(i, 1, &sampler);
    }
    for (int i = 0; i < drawDesc.pD3DTexture2D->size(); i++)
    {
        ID3D11ShaderResourceView* srv = (*drawDesc.pD3DTexture2D)[i];
        pDeviceContext->VSSetShaderResources(i, 1, &srv);
        pDeviceContext->PSSetShaderResources(i, 1, &srv);
    }
 
    DRAW_INDEX_DATA* data = drawDesc.pVertexIndex;
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정점을 이어서 그릴 방식 설정.
    pDeviceContext->IASetVertexBuffers(0, 1, &data->pVertexBuffer, &data->vertexBufferStride, &data->vertexBufferOffset);
    pDeviceContext->IASetIndexBuffer(data->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	// INDEX값의 범위
    pDeviceContext->IASetInputLayout(drawDesc.pInputLayout);
    pDeviceContext->VSSetShader(drawDesc.pVertexShader, nullptr, 0);
    pDeviceContext->PSSetShader(drawDesc.pPixelShader, nullptr, 0);
    pDeviceContext->DrawIndexed(data->indicesCount, 0, 0);
}
