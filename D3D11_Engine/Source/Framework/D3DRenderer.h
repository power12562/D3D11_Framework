#pragma once
#include <Utility\D3D11Utility.h> //helper
#include <Framework\TSingleton.h>
class D3DRenderer : public TSingleton<D3DRenderer>
{
	friend class WinGameApp;
	friend TSingleton;
protected:
	D3DRenderer();
	~D3DRenderer();

private:
	void Init();
	void Uninit();	   

public:
	ID3D11Device* GetDevice() { return pDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return pDeviceContext; }
	IDXGISwapChain* GetSwapChain() { return pSwapChain; }
	ID3D11RenderTargetView* GetRenderTargetView() { return pRenderTargetView; }
	ID3D11DepthStencilView* GetDepthStencilView() { return pDepthStencilView; }

private:
	ID3D11Device*			 pDevice;			  // ����̽�	
	ID3D11DeviceContext*	 pDeviceContext;	  // ����̽� ���ؽ�Ʈ
	IDXGISwapChain*			 pSwapChain;		  // ����ü��
	ID3D11RenderTargetView*  pRenderTargetView;	  // ������ Ÿ�ٺ�
	ID3D11DepthStencilView*  pDepthStencilView;   // ���� ����
};

extern D3DRenderer& Renderer;