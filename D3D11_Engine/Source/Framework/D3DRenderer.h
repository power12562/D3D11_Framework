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
	ID3D11Device*			 pDevice;						// ����̽�	
	ID3D11DeviceContext*	 pDeviceContext;			// ��� ����̽� ���ؽ�Ʈ
	IDXGISwapChain*			 pSwapChain;					// ����ü��
	ID3D11RenderTargetView*  pRenderTargetView;	// ������ Ÿ�ٺ�
	ID3D11DepthStencilView*  pDepthStencilView;   // ���� ����
};

extern D3DRenderer D3D11Renderer;