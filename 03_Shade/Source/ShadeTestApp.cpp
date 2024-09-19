#include "ShadeTestApp.h"
#include "SimpleShadeCube.h"

#include <Math\Mathf.h>
#include <Utility\D3D11Utility.h>
#include <Utility/MemoryUtility.h>
#include <Framework\D3DRenderer.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

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

using namespace Utility;

ShadeTestApp::ShadeTestApp()
{
    this->windowName = L"ShadeTestApp";
    this->customWndProc = WndProc;
}

ShadeTestApp::~ShadeTestApp()
{
   
}

void ShadeTestApp::InitScene()
{
	ID3D11Device* pDevice = D3D11Renderer.GetDevice();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
    XMVECTOR Eye = XMVectorSet(0.0f, 4.0f, -10.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    SimpleShadeObject::mView = XMMatrixLookAtLH(Eye, At, Up);

    // Initialize the projection matrix
    SIZE size = GetClientSize();
    SimpleShadeObject::mProjection = XMMatrixPerspectiveFovLH(90 * Mathf::Deg2Rad, (FLOAT)size.cx / (FLOAT)size.cy, 0.01f, 100.0f);

    m_cubeObject = new SimpleCubeShadeObject;
    m_cubeObject->transform->position = { 0.f, 0.f, 3.0f };
}

void ShadeTestApp::UninitScene()
{
    SafeRelease(m_pVertexShader);
    SafeRelease(m_pPixelShader);
    SafeRelease(m_pInputLayout);
    SafeRelease(m_pConstantBuffer);
    SafeDelete(m_cubeObject);
}

void ShadeTestApp::InitImGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(GetHWND());
    ImGui_ImplDX11_Init(D3D11Renderer.GetDevice(), D3D11Renderer.GetDeviceContext());
}

void ShadeTestApp::UninitImGUI()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ShadeTestApp::Start()
{

}

void ShadeTestApp::Update()
{
    m_cubeObject->Update();
}

void ShadeTestApp::Render()
{
    // 화면 칠하기.
    float bgColor[4] = { 0.4f, 1.0f, 1.0f, 0.0f };
    D3D11Renderer.GetDeviceContext()->ClearRenderTargetView(D3D11Renderer.GetRenderTargetView(), bgColor);
    //깊이 버퍼 초기화
    D3D11Renderer.GetDeviceContext()->ClearDepthStencilView(D3D11Renderer.GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    m_cubeObject->Render(m_pConstantBuffer, m_pInputLayout, m_pVertexShader, m_pPixelShader);

    ImGUIRender();
    // Present the information rendered to the back buffer to the front buffer (the screen)
    D3D11Renderer.GetSwapChain()->Present(0, 0);
}

void ShadeTestApp::ImGUIRender()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        ImGui::Begin("Debug");                          // Create a window called "Hello, world!" and append into it.
        ImGui::Text("Test");
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
