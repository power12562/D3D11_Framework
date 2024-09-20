#pragma once
class D3DRenderer
{
	friend class WinGameApp;
public:
	D3DRenderer();
	~D3DRenderer();

private:
	void Init();
	void Uninit();	   

public:
	struct ID3D11Device* GetDevice() { return pDevice; }
	struct ID3D11DeviceContext* GetDeviceContext() { return pDeviceContext; }
	struct IDXGISwapChain* GetSwapChain() { return pSwapChain; }
	struct ID3D11RenderTargetView* GetRenderTargetView() { return pRenderTargetView; }
	struct ID3D11DepthStencilView* GetDepthStencilView() { return pDepthStencilView; }

private:
	ID3D11Device*			 pDevice;						// 디바이스	
	ID3D11DeviceContext*	 pDeviceContext;			// 즉시 디바이스 컨텍스트
	IDXGISwapChain*			 pSwapChain;					// 스왑체인
	ID3D11RenderTargetView*  pRenderTargetView;	// 렌더링 타겟뷰
	ID3D11DepthStencilView*  pDepthStencilView;   // 깊이 버퍼
};

extern D3DRenderer D3D11Renderer;