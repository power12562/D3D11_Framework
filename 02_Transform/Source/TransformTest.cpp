#include "TransformTest.h"
#include <Utility/D3D11Utility.h>
#include <Utility/MemoryUtility.h>
#include <Math/Mathf.h>
#include <_Debug/Console.h>
#include <Framework/TimeSystem.h>
#include <cassert>
#include "SimpleObject.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

namespace G_ImGUI
{
    bool show_demo_window{};

    float mesh1_pos[3]{};
    float mesh2_pos[3]{ 3.f, 0.f, 0.f };
    float mesh3_pos[3]{ 0.f, 3.f, 0.f };

    float rotation_speed_1{100};
    float rotation_speed_2{75};
    float rotation_speed_3{50};

    float camera_pos[3]{ 0.0f, 1.0f, -5.0f };
    float camera_fov_y{ 90 * Mathf::Deg2Rad };
    float camera_near{0.1f};
    float camera_far{100.f};
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

TransformTest::TransformTest()
{
    this->windowName = L"TransformTest";
    this->clientSize = { 1280, 720 };
    this->customWndProc = WndProc;
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
    using namespace G_ImGUI;
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
        XMVECTOR Eye = XMVectorSet(camera_pos[0], camera_pos[1], camera_pos[2], 0.0f);
        XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

        SimpleObject::mView = XMMatrixLookAtLH(Eye, At, Up);

        // Initialize the projection matrix
        SIZE size = GetClientSize();
        SimpleObject::mProjection = XMMatrixPerspectiveFovLH(camera_fov_y, (FLOAT)size.cx / (FLOAT)size.cy, camera_near, camera_far);
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

bool TransformTest::InitImGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(GetHWND());
    ImGui_ImplDX11_Init(this->pDevice, this->pDeviceContext);

    return true;
}

void TransformTest::UninitImGUI()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void TransformTest::Start()
{                    
    using namespace G_ImGUI;
    CubeObject* cube1 = new CubeObject;

    CubeObject* cube2 = new CubeObject;
    cube2->transform->SetParent(cube1->transform);
    cube2->transform->scale = { 0.5, 0.5, 0.5 };
    cube2->transform->localPosition = { mesh2_pos[0], mesh2_pos[1], mesh2_pos[2]};

    CubeObject* cube3 = new CubeObject;      
    cube3->transform->SetParent(cube2->transform);
    cube3->transform->scale = { 0.25, 0.25, 0.25 };
    cube3->transform->localPosition = { mesh3_pos[0], mesh3_pos[1], mesh3_pos[2]};

    objList.push_back(cube1);
    objList.push_back(cube2);
    objList.push_back(cube3);
}

void TransformTest::Update()
{
    using namespace G_ImGUI;
    using namespace TimeSystem;
    SIZE size = GetClientSize();

    XMVECTOR Eye = XMVectorSet(camera_pos[0], camera_pos[1], camera_pos[2], 0.0f);

    const Vector3& atPosition = objList[0]->transform->position;
    XMVECTOR At = XMVectorSet(atPosition.x, atPosition.y, atPosition.z, 0.0f); //큐브를 향해
    //XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); 

    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    SimpleObject::mView = XMMatrixLookAtLH(Eye, At, Up);
    SimpleObject::mProjection = XMMatrixPerspectiveFovLH(G_ImGUI::camera_fov_y, (FLOAT)size.cx / (FLOAT)size.cy, G_ImGUI::camera_near, G_ImGUI::camera_far);

    objList[0]->transform->position = { mesh1_pos[0], mesh1_pos[1], mesh1_pos[2] };
    objList[1]->transform->position = { mesh2_pos[0], mesh2_pos[1], mesh2_pos[2] };
    objList[2]->transform->position = { mesh3_pos[0], mesh3_pos[1], mesh3_pos[2] };

    objList[0]->transform->rotation += Vector3::Up * rotation_speed_1 * Time.DeltaTime;
    objList[1]->transform->rotation += Vector3::Up * rotation_speed_2 * Time.DeltaTime;
    objList[2]->transform->rotation += Vector3::Up * rotation_speed_3 * Time.DeltaTime;

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

    ImGUIRender();

    // Present the information rendered to the back buffer to the front buffer (the screen)
    pSwapChain->Present(0, 0);
}

void TransformTest::ImGUIRender()
{
    using namespace G_ImGUI;

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        ImGui::Begin("Debug");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

        ImGui::Text("Mesh1");
        ImGui::DragFloat3("mesh1_position", mesh1_pos);
        ImGui::SliderFloat("mesh1_rot_speed", &rotation_speed_1, 0.0f, 300.0f);  

        ImGui::Text("Mesh2");
        ImGui::DragFloat3("mesh2_position", mesh2_pos);
        ImGui::SliderFloat("mesh2_rot_speed", &rotation_speed_2, 0.0f, 300.0f);    

        ImGui::Text("Mesh3");
        ImGui::DragFloat3("mesh3_position", mesh3_pos);
        ImGui::SliderFloat("mesh3_rot_speed", &rotation_speed_3, 0.0f, 300.0f);     

        ImGui::Text("Camera");
        ImGui::DragFloat3("position", camera_pos);
        ImGui::SliderFloat("fov", &camera_fov_y, 10.0f * Mathf::Deg2Rad, 175.0f * Mathf::Deg2Rad);
        ImGui::InputFloat("Near", &camera_near);
        ImGui::InputFloat("Far", &camera_far);
     
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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
