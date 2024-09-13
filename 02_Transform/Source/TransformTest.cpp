#include "TransformTest.h"
#include <Utility/D3D11Utility.h>
#include <Utility/MemoryUtility.h>
#include <Math/Mathf.h>
#include <_Debug/Console.h>
#include <Framework/TimeSystem.h>
#include <cassert>
#include "SimpleObject.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

TransformTest::TransformTest()
{
    this->windowName = L"TransformTest";
    this->clientSize = { 1280, 720 };
}

TransformTest::~TransformTest()
{
    ClearObjList();
}

bool TransformTest::InitD3D()
{
    using namespace Utility;

    DXGI_SWAP_CHAIN_DESC swapDesc{}; //스왑체인 속성 구조체  
    swapDesc.BufferCount = 1; //버퍼 개수
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //버퍼 사용 방식 지정
    swapDesc.OutputWindow = this->GetHWND(); //핸들 윈도우
    swapDesc.Windowed = true; //창모드 유무
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //출력 포멧 지정.

    //버퍼 사이즈 지정
    SIZE size = GetClientSize();
    swapDesc.BufferDesc.Width = size.cx;
    swapDesc.BufferDesc.Height = size.cy;

    //화면 주사율
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;

    //샘플링 설정
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;

    UINT creationFlags = 0;
#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    try
    {
        SIZE clientSize = GetClientSize();

		//디바이스, 스왑체인, 디바이스 컨텍스트 생성
		CheackHRESULT(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
			D3D11_SDK_VERSION, &swapDesc, &pSwapChain, &pDevice, NULL, &pDeviceContext));

        //렌더타겟뷰 생성. (백퍼버 이용)
        ID3D11Texture2D* pBackBufferTexture = nullptr; //백버퍼
        CheackHRESULT(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)); //스왑체인 백버퍼를 가져온다.
        CheackHRESULT(pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &pRenderTargetView)); //백퍼퍼를 참조하는 뷰 생성(참조 카운트 증가.)
        SafeRelease(pBackBufferTexture);
        //출력 파이프라인에 바인딩.
        pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);

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

    }
    catch (const std::exception& ex)
    {
        Debug_printf("%s\n", ex.what());
        assert(!"D3D Init Error");
        return false;
    }
    return true;
}

void TransformTest::UninitD3D()
{
    using namespace Utility;
    SafeRelease(pDevice);
    SafeRelease(pDeviceContext);
    SafeRelease(pSwapChain);
    SafeRelease(pRenderTargetView);
}

bool TransformTest::InitScene()
{
    using namespace Utility;
    try
    {
        //hlsl 입력 레이아웃
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        //버텍스 셰이더 컴파일
        ID3D10Blob* vertexShaderBuffer = nullptr;	// 정점 셰이더 코드가 저장될 버퍼.
        CheackHRESULT(CompileShaderFromFile(L"VertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));
        CheackHRESULT(pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
            vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout));

        //버텍스 셰이더 생성
        CheackHRESULT(pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
            vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader));
        SafeRelease(vertexShaderBuffer);

        // 픽셀 셰이더 컴파일
        ID3D10Blob* pixelShaderBuffer = nullptr;	// 픽셀 셰이더 코드가 저장될 버퍼.
        CheackHRESULT(CompileShaderFromFile(L"PixelShader.hlsl", "main", "ps_4_0", &pixelShaderBuffer));
        // 픽셸 셰이더 생성
        CheackHRESULT(pDevice->CreatePixelShader(
            pixelShaderBuffer->GetBufferPointer(),
            pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
        SafeRelease(pixelShaderBuffer);

        D3D11_BUFFER_DESC bd{};
        // Create the constant buffer
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(ConstantBuffer);
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = 0;
        CheackHRESULT(pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer));

        // Initialize the view matrix
        XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
        XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

        SimpleObject::mView = XMMatrixLookAtLH(Eye, At, Up);

        // Initialize the projection matrix
        SIZE size = GetClientSize();
        SimpleObject::mProjection = XMMatrixPerspectiveFovLH(90 * Mathf::Deg2Rad, (FLOAT)size.cx / (FLOAT)size.cy, 0.1f, 100.0f);
        return true;
    }
    catch (const std::exception& ex)
    {
        Debug_printf("%s\n", ex.what());
        assert(!"InitScene Error");
        return false;
    }
    return true;
}

void TransformTest::UninitScene()
{
    using namespace Utility;
    SafeRelease(m_pVertexShader);
    SafeRelease(m_pPixelShader);
    SafeRelease(m_pInputLayout);
    SafeRelease(m_pConstantBuffer);
}

void TransformTest::Start()
{                        
    CubeObject* cube1 = new CubeObject;

    CubeObject* cube2 = new CubeObject;
    cube2->transform->SetParent(cube1->transform);
    cube2->transform->scale = { 0.5, 0.5, 0.5 };
    cube2->transform->localPosition = { 3.f, 0.f, 0.f };

    CubeObject* cube3 = new CubeObject;      
    cube3->transform->SetParent(cube2->transform);
    cube3->transform->scale = { 0.25, 0.25, 0.25 };
    cube3->transform->localPosition = { 0.f, 3.f, 0.f };

    objList.push_back(cube1);
    objList.push_back(cube2);
    objList.push_back(cube3);
}

void TransformTest::Update()
{
    using namespace TimeSystem;
    objList[0]->transform->rotation += Vector3::Up * 100.f * Time.DeltaTime;
    objList[1]->transform->rotation += Vector3::Right * 75.f * Time.DeltaTime;
    objList[2]->transform->rotation += Vector3::Forward * 50.f * Time.DeltaTime;

    for (auto item : objList)
    {
        item->Update();
    }
}

void TransformTest::Render()
{
    // 화면 칠하기.
    float bgColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    pDeviceContext->ClearRenderTargetView(pRenderTargetView, bgColor);
    //깊이 버퍼 초기화
    pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    for (auto item : objList)
    {
        item->Render(m_pConstantBuffer, m_pInputLayout, m_pVertexShader, m_pPixelShader);
    }
    // Present the information rendered to the back buffer to the front buffer (the screen)
    pSwapChain->Present(0, 0);
}

void TransformTest::ClearObjList()
{
    if (!objList.empty())
    {
        for (auto item : objList)
        {
            delete item;
        }
    }
    objList.clear();
}
