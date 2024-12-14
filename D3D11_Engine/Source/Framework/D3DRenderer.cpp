#include "D3DRenderer.h"
#include <Material\BlingPhongMaterial.h>
#include <dxgi1_4.h>
#include <cassert>
#include <Psapi.h>
#include <framework.h>

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
    pDefaultDepthStencilState = nullptr;
    pSkyBoxDepthStencilState = nullptr;
    pDefaultBlendState = nullptr;
    pDefaultRRState = nullptr;
}

D3DRenderer::~D3DRenderer()
{
   
}

void D3DRenderer::Init()
{
    using namespace Utility;
    try
    {
        //dxgi ����̽�
        CheckHRESULT(CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&pDXGIFactory));

        //��� �׷��� ī�� �� ����� ����. 
        size_t i = 0;
        IDXGIAdapter* pAdapter = nullptr;
        while (pDXGIFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND) //�׷��� ī�� ����)
        {
            CheckHRESULT(pAdapter->QueryInterface(__uuidof(IDXGIAdapter3), reinterpret_cast<void**>(&pAdapter)));
            DXGIAdapters.push_back(reinterpret_cast<IDXGIAdapter3*>(pAdapter));
            DXGIOutputs.resize(i + 1);
            size_t j = 0;
            IDXGIOutput* pOutput = nullptr;
            while (pAdapter->EnumOutputs(j, &pOutput) != DXGI_ERROR_NOT_FOUND)
            {
                CheckHRESULT(pOutput->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&pOutput)));
                DXGIOutputs[i].push_back(reinterpret_cast<IDXGIOutput1*>(pOutput));
                ++j;
            }
            ++i;
        }

        DXGI_ADAPTER_DESC adapterDesc;      
        for (auto& Adapters : DXGIAdapters)
        {
            Adapters->GetDesc(&adapterDesc);
            Debug_wprintf(L"%lu : %s\n", adapterDesc.DeviceId, adapterDesc.Description); //��ī �̸�
        }

        DXGI_OUTPUT_DESC outputDesc;
        DXGIOutputs[0][0]->GetDesc(&outputDesc); //���� �����
        if (outputDesc.AttachedToDesktop)
        {
            Debug_wprintf(L"Moniter 1 : %s\n", outputDesc.DeviceName);
            Debug_wprintf(L"Rotation : %s\n", GetDisplayRotationToCWStr(outputDesc.Rotation));
        }

        DXGI_SWAP_CHAIN_DESC swapDesc{}; //����ü�� �Ӽ� ����ü  
        swapDesc.BufferCount = 2; //���� ����
        swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //���� ��� ��� ����
        swapDesc.OutputWindow = WinGameApp::GetHWND(); //�ڵ� ������
        swapDesc.Windowed = true; //â��� ����
        swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //��� ���� ����.
        swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //�ø� ��� ���.   
        swapDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //��ü ȭ�� ��ȯ�� �ػ� �� ����� ���� �ڵ� ���� �÷���
        swapChainWindowed = swapDesc.Windowed;

        //���� ������ ����
        SIZE clientSize = WinGameApp::GetClientSize();
        swapDesc.BufferDesc.Width = clientSize.cx;
        swapDesc.BufferDesc.Height = clientSize.cy;

        //0/0�� �ڵ� ������. �׸��� â���� ������ ���� �ȵ�...
		swapDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapDesc.BufferDesc.RefreshRate.Denominator = 0;

        //���ø� ���� *(MSAA)
        swapDesc.SampleDesc.Count = 1;
        swapDesc.SampleDesc.Quality = 0;

        UINT creationFlags = 0;
#ifdef _DEBUG
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        //����̽�, ����ü��, ����̽� ���ؽ�Ʈ ����
        IDXGISwapChain* swapChain = nullptr;
        CheckHRESULT(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
            D3D11_SDK_VERSION, &swapDesc, &swapChain, &pDevice, NULL, &pDeviceContext));
  
        CheckHRESULT(swapChain->QueryInterface(IID_PPV_ARGS(&pSwapChain)));
        SafeRelease(swapChain);

        //����Ÿ�ٺ� ����. (���۹� �̿�)
        ID3D11Texture2D* pBackBufferTexture = nullptr; //�����
        CheckHRESULT(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)); //����ü�� ����۸� �����´�.
        if(pBackBufferTexture)
            CheckHRESULT(pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &pRenderTargetView)); //�����۸� �����ϴ� �� ����(���� ī��Ʈ ����.)
        SafeRelease(pBackBufferTexture);

        //������ȭ �⺻ ��Ģ
        D3D11_RASTERIZER_DESC rasterDesc;
        ZeroMemory(&rasterDesc, sizeof(rasterDesc));
        rasterDesc.FillMode = D3D11_FILL_SOLID;
        rasterDesc.CullMode = D3D11_CULL_NONE;  // �ø� ����
        rasterDesc.FrontCounterClockwise = false; // �⺻��
        SetRRState(rasterDesc);

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
        CheckHRESULT(pDevice->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil));

        // Create the depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
        descDSV.Format = descDepth.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        if(textureDepthStencil)
            CheckHRESULT(pDevice->CreateDepthStencilView(textureDepthStencil, &descDSV, &pDepthStencilView));
        SafeRelease(textureDepthStencil);

        // �⺻ ���� ���ٽ� ���� ����
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable = TRUE;                         // ���� �׽�Ʈ Ȱ��ȭ
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // ���� ���� ���� Ȱ��ȭ
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;           // ���� �� �Լ�
        depthStencilDesc.StencilEnable = FALSE;                      // ���ٽ� �׽�Ʈ Ȱ��ȭ
        CheckHRESULT(pDevice->CreateDepthStencilState(&depthStencilDesc, &pDefaultDepthStencilState));

        // ��ī�� �ڽ� ���� ���ٽ� ���� ����
        depthStencilDesc.DepthEnable = FALSE;  // ���� �׽�Ʈ ��Ȱ��ȭ
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // ���� ���� ��Ȱ��ȭ
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_NEVER; // ���� �� �Լ�
        depthStencilDesc.StencilEnable = FALSE;  // ���ٽ� �׽�Ʈ ��Ȱ��ȭ
        CheckHRESULT(pDevice->CreateDepthStencilState(&depthStencilDesc, &pSkyBoxDepthStencilState));

        //Set alpha blend
        D3D11_BLEND_DESC blendDesc;
        ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

        // ���� ���� ����
        blendDesc.AlphaToCoverageEnable = FALSE; // Alpha To Coverage ��Ȱ��ȭ
        blendDesc.IndependentBlendEnable = FALSE; // ���� ���� ��Ȱ��ȭ
        blendDesc.RenderTarget[0].BlendEnable = TRUE; // ���� Ȱ��ȭ
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // �ҽ� ���� (�ҽ� ���� ���)
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // ��� ���� (1 - �ҽ� ����)
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // ���� ����
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // ���� �ҽ� ����
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // ���� ��� ����
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // ���� ���� ����
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // ��� ���� ä�� ���� ���
        CheckHRESULT(pDevice->CreateBlendState(&blendDesc, &pDefaultBlendState));

        SetDefaultOMState();

        //����Ʈ ����
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = (float)clientSize.cx;
        viewport.Height = (float)clientSize.cy;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        //�⺻ ����Ʈ �Ҵ�
        pDeviceContext->RSSetViewports(1, &viewport);
        viewportsCount = 1;
        
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
    //dxgi ��ü
    for (auto& list : DXGIOutputs)
    {
        for(auto& Output : list)
            SafeRelease(Output);
    }
    for (auto& Adapters : DXGIAdapters)
    {
        SafeRelease(Adapters);
    }
    SafeRelease(pDXGIFactory);
     
    //dxd11 ��ü
    D3DConstBuffer::ReleaseStaticCbuffer();
    SafeRelease(pDefaultRRState);
    SafeRelease(pDefaultBlendState);
    SafeRelease(pRenderTargetView);
    SafeRelease(pSkyBoxDepthStencilState);
    SafeRelease(pDefaultDepthStencilState);
    SafeRelease(pDepthStencilView);
    {
        CheckHRESULT(pSwapChain->SetFullscreenState(FALSE, nullptr)); //����ü�� ���� ������ â���� ��ȯ
        SafeRelease(pSwapChain);
        pDeviceContext->ClearState();   //���� ����
        pDeviceContext->Flush();        //GPU ��� ���
    }  
    SafeRelease(pDeviceContext);
    textureManager.ReleaseDefaultTexture();

#ifdef _DEBUG
    //ID3D11Debug* debug = nullptr;
    //pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug));
    //if (debug)
    //{
    //    debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL); // �� ���� ���
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

void D3DRenderer::SetDefaultOMState()
{
    pDeviceContext->OMSetBlendState(pDefaultBlendState, nullptr, 0xFFFFFFFF);
    pDeviceContext->OMSetDepthStencilState(pDefaultDepthStencilState, 0);
}

void D3DRenderer::SetRRState(D3D11_RASTERIZER_DESC& defaultDesc)
{
    Utility::SafeRelease(pDefaultRRState);
    CreateRRState(defaultDesc, &pDefaultRRState);
}

void D3DRenderer::CreateRRState(D3D11_RASTERIZER_DESC& RASTERIZER_DESC, ID3D11RasterizerState** rasterState)
{
    Utility::CheckHRESULT(pDevice->CreateRasterizerState(&RASTERIZER_DESC, rasterState));
}

void D3DRenderer::SetViewports(const std::vector<D3D11_VIEWPORT>& viewPorts)
{
    viewportsCount = (UINT)viewPorts.size();
    pDeviceContext->RSSetViewports(viewportsCount, viewPorts.data());
}

namespace BytesHelp
{
    constexpr UINT64 toMB = 1024 * 1024;  // �ް�����Ʈ ������ ��ȯ��
}

USAGE_VRAM_INFO D3DRenderer::GetLocalVramUsage()
{
    DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
    DXGIAdapters[0]->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);
    USAGE_VRAM_INFO info{};
    info.Budget = videoMemoryInfo.Budget / BytesHelp::toMB;
    info.Usage = videoMemoryInfo.CurrentUsage / BytesHelp::toMB;
    return info;
}

USAGE_VRAM_INFO D3DRenderer::GetNonLocalVramUsage()
{
    DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
    DXGIAdapters[0]->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL, &videoMemoryInfo);

    USAGE_VRAM_INFO info{};
    info.Budget = videoMemoryInfo.Budget / BytesHelp::toMB;
    info.Usage = videoMemoryInfo.CurrentUsage / BytesHelp::toMB;

    return info;
}

SYSTEM_VRAM_INFO D3DRenderer::GetSystemVramInfo()
{
    DXGI_ADAPTER_DESC desc;
    DXGIAdapters[0]->GetDesc(&desc);

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
    Camera* mainCam = Camera::GetMainCamera();
    cbuffer::camera.MainCamPos = mainCam->transform.position;
    cbuffer::camera.View = XMMatrixTranspose(mainCam->GetVM());
    cbuffer::camera.Projection = XMMatrixTranspose(mainCam->GetPM());
    D3DConstBuffer::UpdateStaticCbuffer(cbuffer::camera);

    pDeviceContext->ClearRenderTargetView(pRenderTargetView, backgroundColor);  // ȭ�� ĥ�ϱ�.  
    pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);  //flip ��带 ����ϱ� ������ �� ������ �������־�� �Ѵ�.

    //sky box ���� �׸���.
    if (SkyBoxRender* mainSkybox = SkyBoxRender::GetMainSkyBox())
    {      
        pDeviceContext->OMSetDepthStencilState(pSkyBoxDepthStencilState, 0);
        RENDERER_DRAW_DESC desc = mainSkybox->GetRendererDesc();
        Draw(desc); //SkyBox draw
        pDeviceContext->OMSetDepthStencilState(pDefaultDepthStencilState, 0); //�⺻ ���� ���ٽ� ���� ����.

        constexpr int index2SkyBox = E_TEXTURE::Diffuse_IBL - SkyBoxRender::Diffuse_IBL;
        //SetIBLTexture
        for (int i = E_TEXTURE::Diffuse_IBL; i < E_TEXTURE::Null; ++i)
        {        
            ID3D11ShaderResourceView* srv = mainSkybox->textures[i - index2SkyBox];
            if(srv)
                pDeviceContext->PSSetShaderResources(i, 1, &srv);
        }
    }
    else
    {
        for (int i = E_TEXTURE::Diffuse_IBL; i < E_TEXTURE::BRDF_LUT; ++i)
        {
            ID3D11ShaderResourceView* srv = textureManager.GetDefaultTexture(E_TEXTURE_DEFAULT::CUBE_ZERO);
            pDeviceContext->PSSetShaderResources(i, 1, &srv);
        }
        ID3D11ShaderResourceView* srv = textureManager.GetDefaultTexture(E_TEXTURE_DEFAULT::ZERO);
        pDeviceContext->PSSetShaderResources(E_TEXTURE::BRDF_LUT, 1, &srv);
    }
    pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);  //���� ���� �ʱ�ȭ

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

static const Transform* prevTransform = nullptr; //���������� ������ Trnasform
void D3DRenderer::EndDraw()
{
    prevTransform = nullptr;
    for (auto& item : opaquerenderOueue)
    {     
        Draw(item);
    }
    for (auto& item : alphaRenderQueue)
    {
        Draw(item);
    }
    opaquerenderOueue.clear();
    alphaRenderQueue.clear();
}

void D3DRenderer::Present()
{
    if(setting.UseVSync)
	    pSwapChain->Present(1, 0);
    else
        pSwapChain->Present(0, 0);
}

void D3DRenderer::Draw(RENDERER_DRAW_DESC& drawDesc)
{
    static bool isDefaultRRState = false;
    if (drawDesc.pRRState)
    {
        pDeviceContext->RSSetState(drawDesc.pRRState);
        isDefaultRRState = false;
    }
    else if(!isDefaultRRState)
    {
        pDeviceContext->RSSetState(pDefaultRRState);
        isDefaultRRState = true;
    }

    if (prevTransform != drawDesc.pTransform)
    {
        Matrix VP = Camera::GetMainCamera()->GetVM() * Camera::GetMainCamera()->GetPM();
        const Matrix& WM = drawDesc.pTransform->GetWM();

        cbuffer::transform.World = XMMatrixTranspose(WM);
        cbuffer::transform.WorldInverseTranspose = XMMatrixInverse(nullptr, WM);
        cbuffer::transform.WVP = XMMatrixTranspose(drawDesc.pTransform->GetWM() * VP);
        D3DConstBuffer::UpdateStaticCbuffer(cbuffer::transform);
        prevTransform = drawDesc.pTransform;
    }

    drawDesc.pConstBuffer->UpdateEvent();
    drawDesc.pConstBuffer->SetConstBuffer();

    for (int i = 0; i < drawDesc.pSamperState->size(); i++)
    {
        ID3D11SamplerState* sampler = (*drawDesc.pSamperState)[i];
        //pDeviceContext->VSSetSamplers(i, 1, &sampler);
        pDeviceContext->PSSetSamplers(i, 1, &sampler);
    }
    for (int i = 0; i < drawDesc.pD3DTexture2D->size(); i++)
    {
        ID3D11ShaderResourceView* srv = (*drawDesc.pD3DTexture2D)[i];
        //pDeviceContext->VSSetShaderResources(i, 1, &srv);
        pDeviceContext->PSSetShaderResources(i, 1, &srv);
    }
 
    DRAW_INDEX_DATA* data = drawDesc.pVertexIndex;
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ������ �̾ �׸� ��� ����.
    pDeviceContext->IASetVertexBuffers(0, 1, &data->pVertexBuffer, &data->vertexBufferStride, &data->vertexBufferOffset);
    pDeviceContext->IASetIndexBuffer(data->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	// INDEX���� ����
    pDeviceContext->IASetInputLayout(drawDesc.pInputLayout);
    pDeviceContext->VSSetShader(drawDesc.pVertexShader, nullptr, 0);
    pDeviceContext->PSSetShader(drawDesc.pPixelShader, nullptr, 0);
    pDeviceContext->DrawIndexed(data->indicesCount, 0, 0);
}

DXGI_MODE_DESC1 D3DRenderer::GetDisplayMode(int AdapterIndex, int OutputIndex)
{
    DXGI_MODE_DESC1 targetDesc{}; //�⺻ �ʱ�ȭ �� ���
    targetDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_MODE_DESC1 outDesc{};
    Utility::CheckHRESULT(DXGIOutputs[AdapterIndex][OutputIndex]->FindClosestMatchingMode1(&targetDesc, &outDesc, nullptr));
    return outDesc;
}

std::vector<DXGI_MODE_DESC1> D3DRenderer::GetDisplayModeList(int AdapterIndex, int OutputIndex)
{
    using namespace Utility;
    std::vector<DXGI_MODE_DESC1> descList(1);
    
    //��� ��� ����Ʈ �������� (�ֻ��� ����)
    UINT numModes = 0;
    CheckHRESULT(DXGIOutputs[AdapterIndex][OutputIndex]->GetDisplayModeList1(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, nullptr));

    if (numModes > 1)
    {
        descList.resize(numModes);
        CheckHRESULT(DXGIOutputs[AdapterIndex][OutputIndex]->GetDisplayModeList1(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, descList.data()));
    }
    return descList;
}

void D3DRenderer::ToggleFullscreenMode()
{
    HWND hwnd = WinGameApp::GetHWND();
    SIZE clientSize = WinGameApp::GetClientSize();
    if (swapChainWindowed)
    {   
        DXGI_SWAP_CHAIN_DESC1 swapDesc{}; //����ü�� �Ӽ� ����ü  
        swapDesc.BufferCount = 2; //���� ����
        swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //���� ��� ��� ����
        swapDesc.Stereo = false; //���׷��� ����
        swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //��� ���� ����.
        swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;      //�ø� ���.
        swapDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //��ü ȭ�� ��ȯ�� �ػ� �� ����� ���� �ڵ� ���� �÷���
   
        //���� ũ�� ����
        swapDesc.Width = clientSize.cx;
        swapDesc.Height = clientSize.cy;

        //���ø� ���� *(MSAA)
        swapDesc.SampleDesc.Count = 1;
        swapDesc.SampleDesc.Quality = 0;

        UINT creationFlags = 0;
#ifdef _DEBUG
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        swapChainWindowed = !swapChainWindowed;
        ReCreateSwapChain(&swapDesc);
        WinGameApp::WinToScreenCenter(hwnd);
    }
    else
    {     
        DXGI_SWAP_CHAIN_DESC1 swapDesc{}; //����ü�� �Ӽ� ����ü  
        swapDesc.BufferCount = 2; //���� ����
        swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //���� ��� ��� ����
        swapDesc.Stereo = false; //���׷��� ����
        swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //��� ���� ����.
        swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;      //�ø� ���.
        swapDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //��ü ȭ�� ��ȯ�� �ػ� �� ����� ���� �ڵ� ���� �÷���

        //���� ������ ����
        swapDesc.Width = clientSize.cx;
        swapDesc.Height = clientSize.cy;

        //���ø� ���� *(MSAA)
        swapDesc.SampleDesc.Count = 1;
        swapDesc.SampleDesc.Quality = 0;

        UINT creationFlags = 0;
#ifdef _DEBUG
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        swapChainWindowed = !swapChainWindowed;
        ReCreateSwapChain(&swapDesc);
        WinGameApp::WinToScreenCenter(hwnd);
    }
}

void D3DRenderer::ReCreateSwapChain(DXGI_SWAP_CHAIN_DESC1* swapChainDesc)
{
    if (pSwapChain)
    {    
        DXGI_SWAP_CHAIN_DESC1 pervDesc;
        CheckHRESULT(pSwapChain->GetDesc1(&pervDesc));
        float resolutionScaleX = (float)swapChainDesc->Width / (float)pervDesc.Width;
        float resolutionScaleY = (float)swapChainDesc->Height / (float)pervDesc.Height;

        std::vector<D3D11_VIEWPORT> lastViewports(viewportsCount);
        pDeviceContext->RSGetViewports(&viewportsCount, lastViewports.data());

        pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr); //����Ÿ�� ���ε� ����
        unsigned long ref = SafeRelease(pRenderTargetView); //RTV ���ҽ� ����
        CheckHRESULT(pSwapChain->SetFullscreenState(FALSE, NULL));
   
        ref = SafeRelease(pSwapChain);
        pDeviceContext->ClearState();   //���� ����
        pDeviceContext->Flush();        //GPU ��� ���

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc{};
        fullScreenDesc.Windowed = swapChainWindowed;
        CheckHRESULT(pDXGIFactory->CreateSwapChainForHwnd(pDevice, WinGameApp::GetHWND(), swapChainDesc, &fullScreenDesc, nullptr, &pSwapChain));
        CheckHRESULT(pSwapChain->SetFullscreenState(!swapChainWindowed, NULL));
        CheckHRESULT(pSwapChain->ResizeBuffers(2, swapChainDesc->Width, swapChainDesc->Height, swapChainDesc->Format, swapChainDesc->BufferUsage));

        //����Ÿ�ٺ� �� ����
        ID3D11Texture2D* pBackBufferTexture = nullptr; //�����
        CheckHRESULT(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)); //����ü�� ����۸� �����´�.
        if (pBackBufferTexture)
            CheckHRESULT(pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &pRenderTargetView)); //�����۸� �����ϴ� �� ����(���� ī��Ʈ ����.)
        SafeRelease(pBackBufferTexture);      

        SetDefaultOMState();  
        
        //����Ʈ ũ�� ������
        for (auto& viewport : lastViewports)
        {
            viewport.Width = std::round(viewport.Width * resolutionScaleX);
            viewport.Height = std::round(viewport.Height * resolutionScaleY);
        }
        SetViewports(lastViewports);
    }
}

const wchar_t* D3DRenderer::GetDisplayRotationToCWStr(DXGI_MODE_ROTATION rotation)
{
    switch (rotation)
    {
    case DXGI_MODE_ROTATION_UNSPECIFIED:
        return L"UNSPECIFIED";
    case DXGI_MODE_ROTATION_IDENTITY:
        return L"IDENTITY";
    case DXGI_MODE_ROTATION_ROTATE90:
        return L"ROTATE90";
    case DXGI_MODE_ROTATION_ROTATE180:
        return L"ROTATE180";
    case DXGI_MODE_ROTATION_ROTATE270:
        return L"ROTATE270";
    default:
        return L"UNKNOWN";
    }
}
