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

	/** Vram 사용량 확인용.*/
	USAGE_VRAM_INFO GetLocalVramUsage();
	/** Vram 사용량 확인용.*/
	USAGE_VRAM_INFO GetNonLocalVramUsage();
	/** gpu 메모리 영역 확인용*/
	SYSTEM_VRAM_INFO GetSystemVramInfo();
	/** 시스템 메모리 영역 확인용*/
	SYSTEM_MEMORY_INFO GetSystemMemoryInfo();

	void reserveRenderQueue(size_t size);

	/*레스터화 기본 규칙 설정*/
	void SetRRState(D3D11_RASTERIZER_DESC& defaultDesc);

	/*레스터화 규칙 생성*/
	void CreateRRState(D3D11_RASTERIZER_DESC& RASTERIZER_DESC, ID3D11RasterizerState** rasterState);
private:
	//Vram 체크용 dxgi 개체
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
	ID3D11Device*			 pDevice;			  // 디바이스	
	ID3D11DeviceContext*	 pDeviceContext;	  // 디바이스 컨텍스트
	IDXGISwapChain*			 pSwapChain;		  // 스왑체인
	ID3D11RenderTargetView*  pRenderTargetView;	  // 렌더링 타겟뷰
	ID3D11DepthStencilView*  pDepthStencilView;   // 깊이 버퍼
	ID3D11BlendState*		 pBlendState;		  // 블렌드 상태
	ID3D11RasterizerState*   pDefaultRRState;    // 기본 레스터화 규칙

private:
	std::vector<RENDERER_DRAW_DESC> opaquerenderOueue; //불투명 오브젝트
	std::vector<RENDERER_DRAW_DESC> alphaRenderQueue;  //반투명 오브젝트
	void Draw(RENDERER_DRAW_DESC& drawDesc);

};

struct USAGE_VRAM_INFO
{
	/*할당 가능한 크기 / MB*/
	UINT64 Budget;

	/*사용중인 메모리 / MB*/
	UINT64 Usage;
};

struct SYSTEM_VRAM_INFO
{
	/*공유 GPU 메모리 영역 / MB*/
	SIZE_T SharedSystemMemory;

	/*전용 GPU 메모리 영역 / MB*/
	SIZE_T DedicatedVideoMemory;
};

struct SYSTEM_MEMORY_INFO
{
	/*현재 할당중인 Private bytes 사이즈*/
	SIZE_T PrivateUsage;

	/*실제 사용중인 Working Set 사이즈*/
	SIZE_T WorkingSetSize;
	
	/*메모리 커밋 사이즈*/
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

