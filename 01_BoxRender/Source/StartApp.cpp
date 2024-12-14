#include "StartApp.h"
#include <Utility/MemoryUtility.h>
#include <Utility/D3D11Utility.h>
#include <directxtk/simplemath.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace Utility;
using namespace DirectX::SimpleMath;

D3D11_BoxDrawApp::D3D11_BoxDrawApp()
{
    this->windowName = L"D3D11_BoxDrawApp";
	this->clientSize = { 1024, 768 };

}

D3D11_BoxDrawApp::~D3D11_BoxDrawApp()
{
	UninitD3D();
	UninitScene();
}

void D3D11_BoxDrawApp::Start()
{
}

void D3D11_BoxDrawApp::Update()
{

}

void D3D11_BoxDrawApp::Render()
{
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// 화면 칠하기.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	// Draw계열 함수를 호출하기전에 렌더링 파이프라인에 필수 스테이지 설정을 해야한다.	
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정점을 이어서 그릴 방식 설정.
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertextBufferStride, &m_VertextBufferOffset);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	// Render a triangle	
	m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);

	// Present the information rendered to the back buffer to the front buffer (the screen)
	m_pSwapChain->Present(0, 0);
}


bool D3D11_BoxDrawApp::InitD3D()
{
	try
	{
		// 결과값.
		HRESULT hr = 0;

		// 스왑체인 속성 설정 구조체 생성.
		DXGI_SWAP_CHAIN_DESC swapDesc = {};
		swapDesc.BufferCount = 1;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow = GetHWND();	// 스왑체인 출력할 창 핸들 값.
		swapDesc.Windowed = true;		// 창 모드 여부 설정.
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// 백버퍼(텍스처)의 가로/세로 크기 설정.
		const SIZE& clientSize = GetClientSize();
		swapDesc.BufferDesc.Width = clientSize.cx;
		swapDesc.BufferDesc.Height = clientSize.cy;
		// 화면 주사율 설정.
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		// 샘플링 관련 설정.
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;

		UINT creationFlags = 0;
#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// 1. 장치 생성.   2.스왑체인 생성. 3.장치 컨텍스트 생성.
		CheckHRESULT(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
			D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext));

		// 4. 렌더타겟뷰 생성.  (백버퍼를 이용하는 렌더타겟뷰)	
		ID3D11Texture2D* pBackBufferTexture = nullptr;
		CheckHRESULT(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture));

		if(pBackBufferTexture)
			CheckHRESULT(m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &m_pRenderTargetView));  //텍스처는 내부 참조 증가
		SafeRelease(pBackBufferTexture);	//외부 참조 카운트를 감소시킨다.
		// 렌더 타겟을 최종 출력 파이프라인에 바인딩합니다.
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

		// 뷰포트 설정.	
		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)clientSize.cx;
		viewport.Height = (float)clientSize.cy;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		m_pDeviceContext->RSSetViewports(1, &viewport);
		return true;

	}
	catch (const std::exception& ex)
	{
		MessageBoxA(nullptr, ex.what(), "Error InitD3D", MB_OK);
		return false;
	}

}

void D3D11_BoxDrawApp::UninitD3D()
{
    SafeRelease(m_pRenderTargetView);
    SafeRelease(m_pSwapChain);
    SafeRelease(m_pDeviceContext);
    SafeRelease(m_pDevice);
}


// 정점 선언.
struct Vertex
{
	Vector3 position;		// 위치 정보.
	Vector4 Color;
};

bool D3D11_BoxDrawApp::InitScene()
{
	try
	{
		HRESULT hr = 0; // 결과값.
		ID3D10Blob* errorMessage = nullptr;	 // 컴파일 에러 메시지가 저장될 버퍼.	

		// 1. Render() 에서 파이프라인에 바인딩할 버텍스 버퍼및 버퍼 정보 준비
		// Normalized Device Coordinate
		//   0-----1
		//   |    /|
		//   |  /  |                중앙이 (0,0)  왼쪽이 (-1,0) 오른쪽이 (1,0) , 위쪽이 (0,1) 아래쪽이 (0,-1)
		//   |/    |
		//	 2-----3
		Vertex vertices[] =
		{
			{Vector3(-0.5f,  0.5f, 0.5f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)},
			{Vector3(0.5f,  0.5f, 0.5f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
			{Vector3(-0.5f, -0.5f, 0.5f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)},
			{Vector3(0.5f, -0.5f, 0.5f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) }
		};

		D3D11_BUFFER_DESC vbDesc = {};
		m_VertexCount = ARRAYSIZE(vertices);
		vbDesc.ByteWidth = sizeof(Vertex) * m_VertexCount;
		vbDesc.CPUAccessFlags = 0;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.MiscFlags = 0;
		vbDesc.Usage = D3D11_USAGE_DEFAULT;

		// 정점 버퍼 생성.
		D3D11_SUBRESOURCE_DATA vbData = {};
		vbData.pSysMem = vertices;
		CheckHRESULT(hr = m_pDevice->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer));

		// 버텍스 버퍼 정보 
		m_VertextBufferStride = sizeof(Vertex);
		m_VertextBufferOffset = 0;


		// 2. Render() 에서 파이프라인에 바인딩할 InputLayout 생성 	
		D3D11_INPUT_ELEMENT_DESC layout[] =  // 인풋 레이아웃은 버텍스 쉐이더가 입력받을 데이터의 형식을 지정한다.
		{// SemanticName , SemanticIndex , Format , InputSlot , AlignedByteOffset , InputSlotClass , InstanceDataStepRate		
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		ID3DBlob* vertexShaderBuffer = nullptr;
		CheckHRESULT(CompileShaderFromFile(L"BasicVertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));
		CheckHRESULT(hr = m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
			vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout));

		// 3. Render에서 파이프라인에 바인딩할  버텍스 셰이더 생성
		CheckHRESULT(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
			vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader));
		SafeRelease(vertexShaderBuffer);


		// 4. Render에서 파이프라인에 바인딩할 픽셀 셰이더 생성
		ID3DBlob* pixelShaderBuffer = nullptr;
		CheckHRESULT(CompileShaderFromFile(L"BasicPixelShader.hlsl", "main", "ps_4_0", &pixelShaderBuffer));
		CheckHRESULT(m_pDevice->CreatePixelShader(
			pixelShaderBuffer->GetBufferPointer(),
			pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
		SafeRelease(pixelShaderBuffer);

		// 5. Render() 에서 파이프라인에 바인딩할 인덱스 버퍼 생성
		UINT indices[] =
		{
			0, 1, 2,
			2, 1, 3,
		};
		m_nIndices = ARRAYSIZE(indices);	// 인덱스 개수 저장.
		D3D11_BUFFER_DESC ibDesc = {};
		ibDesc.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		D3D11_SUBRESOURCE_DATA ibData = {};
		ibData.pSysMem = indices;
		CheckHRESULT(m_pDevice->CreateBuffer(&ibDesc, &ibData, &m_pIndexBuffer));

		return true;
	}
	catch (const std::exception& ex)
	{			   
		MessageBoxA(nullptr, ex.what(), "Error InitScene", MB_OK);
		return false;
	}
}

void D3D11_BoxDrawApp::UninitScene()
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexShader);
	SafeRelease(m_pPixelShader);
	SafeRelease(m_pIndexBuffer);
}

