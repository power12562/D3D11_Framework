#pragma once
#include <Core/WinGameApp.h>
#include <d3d11.h>

class D3D11_BoxDrawApp : public WinGameApp
{
public:
	D3D11_BoxDrawApp();
	~D3D11_BoxDrawApp();

	bool InitD3D();
	void UninitD3D();

	bool InitScene();	
	void UninitScene();

protected:
	virtual void Start();
	virtual void Update();
	virtual void Render();


private:
	// 렌더링 파이프라인을 구성하는 필수 객체의 인터페이스 (  뎊스 스텐실 뷰도 있지만 아직 사용하지 않는다.)
	ID3D11Device* m_pDevice = nullptr;						// 디바이스	
	ID3D11DeviceContext* m_pDeviceContext = nullptr;		// 즉시 디바이스 컨텍스트
	IDXGISwapChain* m_pSwapChain = nullptr;					// 스왑체인
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;	// 렌더링 타겟뷰

	// 렌더링 파이프라인에 적용하는  객체와 정보
	ID3D11VertexShader* m_pVertexShader = nullptr;	// 정점 셰이더.
	ID3D11PixelShader* m_pPixelShader = nullptr;	// 픽셀 셰이더.	
	ID3D11InputLayout* m_pInputLayout = nullptr;	// 입력 레이아웃.
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// 버텍스 버퍼.
	UINT m_VertextBufferStride = 0;					// 버텍스 하나의 크기.
	UINT m_VertextBufferOffset = 0;					// 버텍스 버퍼의 오프셋.
	UINT m_VertexCount = 0;							// 버텍스 개수.

	ID3D11Buffer* m_pIndexBuffer = nullptr;			// 인덱스 버퍼
	UINT m_nIndices = 0;							// 인덱스 개수						

};

