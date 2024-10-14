#pragma once
#include <Utility\D3D11Utility.h> //helper
#include <Utility/MemoryUtility.h>
#include <Framework\TSingleton.h>
#include <map>
#include <string>
#include <cassert>

class D3DRenderer;
class D3DConstBuffer;
extern D3DRenderer& d3dRenderer;

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
	ID3D11Device*			GetDevice() { return pDevice; }
	ID3D11DeviceContext*	GetDeviceContext() { return pDeviceContext; }
	IDXGISwapChain*			GetSwapChain() { return pSwapChain; }
	ID3D11RenderTargetView* GetRenderTargetView() { return pRenderTargetView; }
	ID3D11DepthStencilView* GetDepthStencilView() { return pDepthStencilView; }

public:
	void BegineDraw();
	void EndDraw();

public:
	void DrawIndex(DRAW_INDEX_DATA& data);
	void DrawIndex(DRAW_INDEX_DATA& data, D3DConstBuffer& cbuffer);

public:
	DirectX::SimpleMath::Color backgroundColor{ 0,0,0,1 };

private:
	ID3D11Device*			 pDevice;			  // ����̽�	
	ID3D11DeviceContext*	 pDeviceContext;	  // ����̽� ���ؽ�Ʈ
	IDXGISwapChain*			 pSwapChain;		  // ����ü��
	ID3D11RenderTargetView*  pRenderTargetView;	  // ������ Ÿ�ٺ�
	ID3D11DepthStencilView*  pDepthStencilView;   // ���� ����
	ID3D11BlendState*		 pBlendState;		  // ���� ����

};