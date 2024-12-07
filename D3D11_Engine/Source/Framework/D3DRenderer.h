#pragma once
#include <Utility\D3D11Utility.h> //helper
#include <Utility/MemoryUtility.h>
#include <Framework\TSingleton.h>
#include <map>
#include <string>
#include <cassert>

class Transform;
class D3DRenderer;
class D3DConstBuffer;
class D3DSamplerState;
class D3DTexture2D;
class SimpleMaterial;
extern D3DRenderer& d3dRenderer;

struct USAGE_VRAM_INFO;
struct SYSTEM_VRAM_INFO;
struct SYSTEM_MEMORY_INFO;
struct RENDERER_DRAW_DESC;
struct RENDERER_SETTING_DESC
{
	bool UseVSync = true;
};

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

	/** Vram ��뷮 Ȯ�ο�.*/
	USAGE_VRAM_INFO GetLocalVramUsage();
	/** Vram ��뷮 Ȯ�ο�.*/
	USAGE_VRAM_INFO GetNonLocalVramUsage();
	/** gpu �޸� ���� Ȯ�ο�*/
	SYSTEM_VRAM_INFO GetSystemVramInfo();
	/** �ý��� �޸� ���� Ȯ�ο�*/
	SYSTEM_MEMORY_INFO GetSystemMemoryInfo();

	void reserveRenderQueue(size_t size);

	/*������ȭ �⺻ ��Ģ ����*/
	void SetRRState(D3D11_RASTERIZER_DESC& defaultDesc);

	/*������ȭ ��Ģ ����*/
	void CreateRRState(D3D11_RASTERIZER_DESC& RASTERIZER_DESC, ID3D11RasterizerState** rasterState);
private:
	//Vram üũ�� dxgi ��ü
	struct IDXGIFactory*  pDXGIFactory = nullptr;        
	struct IDXGIAdapter3* pDXGIAdapter = nullptr;

public:
	void BegineDraw();
	void DrawIndex(RENDERER_DRAW_DESC& darwDesc, bool isAlpha);
	void EndDraw();
	void Present();

public:
	DirectX::SimpleMath::Color backgroundColor{ 0,0,0,1 };
	RENDERER_SETTING_DESC setting;

private:
	ID3D11Device*			 pDevice;			  // ����̽�	
	ID3D11DeviceContext*	 pDeviceContext;	  // ����̽� ���ؽ�Ʈ
	IDXGISwapChain*			 pSwapChain;		  // ����ü��
	ID3D11RenderTargetView*  pRenderTargetView;	  // ������ Ÿ�ٺ�
	ID3D11DepthStencilView*  pDepthStencilView;   // ���� ����
	ID3D11BlendState*		 pBlendState;		  // ���� ����
	ID3D11RasterizerState*   pDefaultRRState;    // �⺻ ������ȭ ��Ģ

private:
	std::vector<RENDERER_DRAW_DESC> opaquerenderOueue; //������ ������Ʈ
	std::vector<RENDERER_DRAW_DESC> alphaRenderQueue;  //������ ������Ʈ
	void Draw(RENDERER_DRAW_DESC& drawDesc);

};

struct USAGE_VRAM_INFO
{
	/*�Ҵ� ������ ũ�� / MB*/
	UINT64 Budget;

	/*������� �޸� / MB*/
	UINT64 Usage;
};

struct SYSTEM_VRAM_INFO
{
	/*���� GPU �޸� ���� / MB*/
	SIZE_T SharedSystemMemory;

	/*���� GPU �޸� ���� / MB*/
	SIZE_T DedicatedVideoMemory;
};

struct SYSTEM_MEMORY_INFO
{
	/*���� �Ҵ����� Private bytes ������*/
	SIZE_T PrivateUsage;

	/*���� ������� Working Set ������*/
	SIZE_T WorkingSetSize;
	
	/*�޸� Ŀ�� ������*/
	SIZE_T PagefileUsage;
};

struct RENDERER_DRAW_DESC 
{
	DRAW_INDEX_DATA* pVertexIndex;
	D3DConstBuffer*  pConstBuffer;
	D3DTexture2D*	 pD3DTexture2D;
	D3DSamplerState* pSamperState;
	const Transform* pTransform;
	ID3D11InputLayout* pInputLayout;
	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3D11RasterizerState* pRRState = nullptr;
};

