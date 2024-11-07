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
class SimpleMaterial;
extern D3DRenderer& d3dRenderer;

struct USAGE_VRAM_INFO;
struct SYSTEM_VRAM_INFO;
struct SYSTEM_MEMORY_INFO;

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
private:
	//Vram üũ�� dxgi ��ü
	struct IDXGIFactory*  pDXGIFactory = nullptr;        
	struct IDXGIAdapter3* pDXGIAdapter = nullptr;

public:
	void BegineDraw();
	void DrawIndex(DRAW_INDEX_DATA& data, SimpleMaterial& material, Transform& transform);
	void EndDraw();
	void Present() { pSwapChain->Present(0, 0); }

public:
	DirectX::SimpleMath::Color backgroundColor{ 0,0,0,1 };

private:
	ID3D11Device*			 pDevice;			  // ����̽�	
	ID3D11DeviceContext*	 pDeviceContext;	  // ����̽� ���ؽ�Ʈ
	IDXGISwapChain*			 pSwapChain;		  // ����ü��
	ID3D11RenderTargetView*  pRenderTargetView;	  // ������ Ÿ�ٺ�
	ID3D11DepthStencilView*  pDepthStencilView;   // ���� ����
	ID3D11BlendState*		 pBlendState;		  // ���� ����

private:
	std::vector<std::tuple<DRAW_INDEX_DATA*, SimpleMaterial*, const Transform*>> opaquerenderOueue; //������ ������Ʈ
	std::vector<std::tuple<DRAW_INDEX_DATA*, SimpleMaterial*, const Transform*>> alphaRenderQueue;  //������ ������Ʈ
	void Draw(DRAW_INDEX_DATA* data, SimpleMaterial* material);

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