#include "BlinnPhongTestApp.h"
#include "SimpleBlinnPhongObject.h"

#include <_Debug\Console.h>
#include <Math\Mathf.h>
#include <Utility\D3D11Utility.h>
#include <Utility/MemoryUtility.h>
#include <Framework\D3DRenderer.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

namespace global
{
    float camPos[3]{0.0f, 0.0f, -10.0f};
    float camFOV = 90;
    float cubeRotation[3]{ 0,0,0 };
    float cubeScale{4.0f};
    float bgColor[4]{ 0.4f, 1.0f, 1.0f, 0.0f };;

    float LightDir[4]{ 0,0,1,0 };
    float LightDiffuse[4]{1,1,1,1};
    float LightAmbient[4]{0,0,0,0};
    float LightSpecular[4]{1,1,1,1};

    float MaterialAmbient[4]{ 1,1,1,1 };
    float MaterialDiffuse[4]{ 1,1,1,1 };
    float MaterialSpecular[4]{ 1,1,1,1 };
    float MaterialSpecularPower{ 500 };
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

using namespace Utility;

BlinnPhongTestApp::BlinnPhongTestApp()
{
    this->windowName = L"BlinnPhong";
    this->customWndProc = WndProc;
    
}

BlinnPhongTestApp::~BlinnPhongTestApp()
{
   
}

void BlinnPhongTestApp::InitScene()
{
	ID3D11Device* pDevice = Renderer.GetDevice();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

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

	m_cubeObject = new SimpleBlinnPhongObject;
	m_cubeObject->transform->position = { 0.f, 0.f, 3.0f };
	//m_cubeObject->transform->scale = { 2,1,1 };

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(global::camPos[0], global::camPos[1], global::camPos[2], 0.0f);
	const Vector3& atVector = m_cubeObject->transform->position;
	XMVECTOR At = XMVectorSet(atVector.x, atVector.y, atVector.z, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	SimpleShadeObject::mView = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	SIZE size = GetClientSize();
	SimpleShadeObject::mProjection = XMMatrixPerspectiveFovLH(global::camFOV * Mathf::Deg2Rad, (FLOAT)size.cx / (FLOAT)size.cy, 0.01f, 100.0f);
}

void BlinnPhongTestApp::UninitScene()
{
    SafeRelease(m_pVertexShader);
    SafeRelease(m_pPixelShader);
    SafeRelease(m_pInputLayout);
    SafeRelease(m_pConstantBuffer);
    SafeDelete(m_cubeObject);
}

void BlinnPhongTestApp::InitImGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(GetHWND());
    ImGui_ImplDX11_Init(Renderer.GetDevice(), Renderer.GetDeviceContext());
}

void BlinnPhongTestApp::UninitImGUI()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void BlinnPhongTestApp::Start()
{

}

void BlinnPhongTestApp::Update()
{
    XMVECTOR Eye = XMVectorSet(global::camPos[0], global::camPos[1], global::camPos[2], 0.0f);
    XMVECTOR To = XMVectorSet(0, 0, 1.0, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    SimpleShadeObject::mView = XMMatrixLookToLH(Eye, To, Up);

    SIZE size = GetClientSize();
    SimpleShadeObject::mProjection = XMMatrixPerspectiveFovLH(global::camFOV * Mathf::Deg2Rad, (FLOAT)size.cx / (FLOAT)size.cy, 0.01f, 100.0f);

    m_cubeObject->transform->rotation = Vector3(global::cubeRotation);
    m_cubeObject->Update();
}

void BlinnPhongTestApp::Render()
{
    // 화면 칠하기.
    Renderer.GetDeviceContext()->ClearRenderTargetView(Renderer.GetRenderTargetView(), global::bgColor);
    //깊이 버퍼 초기화
    Renderer.GetDeviceContext()->ClearDepthStencilView(Renderer.GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    m_cubeObject->Render(m_pConstantBuffer, m_pInputLayout, m_pVertexShader, m_pPixelShader);

    ImGUIRender();
    // Present the information rendered to the back buffer to the front buffer (the screen)
    Renderer.GetSwapChain()->Present(0, 0);
}

void BlinnPhongTestApp::ImGUIRender()
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
        ImGui::Begin("Debug");                         
        ImGui::Text("Camera");
        ImGui::DragFloat3("position", global::camPos);
        ImGui::SliderFloat("FOV", &global::camFOV, 10, 120);
        ImGui::Text("");
        ImGui::Text("Cube");
        ImGui::DragFloat3("Rotation", global::cubeRotation);
        ImGui::SliderFloat("Scale", &global::cubeScale, 1.0f, 10.0f);
        ImGui::Text("");
        ImGui::Text("Light");
        ImGui::DragFloat3("LightDir", global::LightDir, 0.01f, -1.0f, 1.0f);
        ImGui::ColorEdit3("LightDiffuse", global::LightDiffuse);
        ImGui::ColorEdit3("LightAmbient", global::LightAmbient);
        ImGui::ColorEdit3("LightSpecular", global::LightSpecular);
        ImGui::Text("");
        ImGui::Text("Material");
        ImGui::ColorEdit3("MaterialDiffuse", global::MaterialDiffuse);
        ImGui::ColorEdit3("MaterialAmbient", global::MaterialAmbient);
        ImGui::ColorEdit3("MaterialSpecular", global::MaterialSpecular);
        ImGui::DragFloat("MaterialSpecularPower", &global::MaterialSpecularPower, 10, 100, 1000);
        ImGui::Text("");
        ImGui::Text("Background");
        ImGui::ColorEdit3("BgColor", global::bgColor);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
