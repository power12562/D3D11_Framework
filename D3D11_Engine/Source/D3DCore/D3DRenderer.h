#pragma once
#include <Utility\D3D11Utility.h> //helper
#include <Utility\MemoryUtility.h>
#include <Core/TSingleton.h>
#include <map>
#include <string>
#include <cassert>
#include <Light/PBRDirectionalLight.h>

constexpr int SHADOW_MAP_SIZE = 8192;
class GameObject;
class Transform;
class D3DRenderer;
class D3DConstBuffer;
class D3DSamplerState;
class D3DTexture2D;
struct IDXGIFactory4;
struct IDXGIAdapter3;
struct IDXGIOutput;
extern D3DRenderer& d3dRenderer;

struct USAGE_VRAM_INFO;
struct SYSTEM_VRAM_INFO;
struct SYSTEM_MEMORY_INFO;
struct RENDERER_DRAW_DESC;
struct RENDERER_SETTING_DESC
{
	//수직 동기화 사용 여부
	bool UseVSync = false;

	//사용할 RTV 개수. [0]번은 스왑체인 백버퍼이다.
	UINT RTVCount = 1; 
};

class D3DRenderer : public TSingleton<D3DRenderer>
{
	friend class WinGameApp;
	friend TSingleton;
public:
	enum
	{
		SHADOW_SRV0 = 128 - cb_PBRDirectionalLight::MAX_LIGHT_COUNT,
		SHADOW_SRV1,
		SHADOW_SRV2,
		SHADOW_SRV3
	};
protected:
	D3DRenderer();
	~D3DRenderer();

private:
	void Init();
	void Uninit();	   
	
public:
	ID3D11Device*				GetDevice() { return pDevice; }
	ID3D11DeviceContext*		GetDeviceContext() { return pDeviceContext; }
	IDXGISwapChain1*			GetSwapChain() { return pSwapChain; }
	ID3D11RenderTargetView*		GetBackBufferRTV() { return pRenderTargetViewArray[0]; }
	ID3D11RenderTargetView*		GetRTV(int index) { return pRenderTargetViewArray[index]; }
	ID3D11DepthStencilView*		GetDepthStencilView() { return pDepthStencilView; }
	ID3D11ShaderResourceView*	GetShadowMapSRV(int index) { return pShadowMapSRV[index]; }

	/** Vram 사용량 확인용.*/
	USAGE_VRAM_INFO GetLocalVramUsage();
	/** Vram 사용량 확인용.*/
	USAGE_VRAM_INFO GetNonLocalVramUsage();
	/** gpu 메모리 영역 확인용*/
	SYSTEM_VRAM_INFO GetSystemVramInfo();
	/** 시스템 메모리 영역 확인용*/
	SYSTEM_MEMORY_INFO GetSystemMemoryInfo();

	void reserveRenderQueue(size_t size); 

	/**기본 상태로 OM 상태 설정 */
	void SetDefaultOMState();

	/*레스터화 기본 규칙 설정*/
	void SetRRState(D3D11_RASTERIZER_DESC& defaultDesc);

	/*레스터화 규칙 생성*/
	void CreateRRState(D3D11_RASTERIZER_DESC& RASTERIZER_DESC, ID3D11RasterizerState** rasterState);

private:
	//Vram 체크용 dxgi 개체
	IDXGIFactory4* pDXGIFactory = nullptr;
	std::vector<IDXGIAdapter3*> DXGIAdapters;
	std::vector<std::vector<IDXGIOutput1*>> DXGIOutputs;

public:
	void BegineDraw();
	void DrawIndex(RENDERER_DRAW_DESC& darwDesc);
	void EndDraw();
	void Present();

private:
	void CheckUpdateTransform(const Transform* pTransform);
	std::vector<Transform*> transformUpdateList;
public:
	std::vector<D3D11_VIEWPORT> ViewPortsVec;				// 뷰포트들
	DirectX::SimpleMath::Color backgroundColor{ 0,0,0,1 };
	RENDERER_SETTING_DESC setting;
private:
	ID3D11Device*				pDevice;					// 디바이스	
	ID3D11DeviceContext*		pDeviceContext;				// 디바이스 컨텍스트
	IDXGISwapChain1*			pSwapChain;					// 스왑체인
	ID3D11RenderTargetView*		pRenderTargetViewArray[8]{};// RTV Array
	ID3D11DepthStencilView*		pDepthStencilView;			// 깊이 버퍼

	ID3D11DepthStencilState*	pDefaultDepthStencilState;  // 기본 상태
	ID3D11DepthStencilState*	pSkyBoxDepthStencilState;   // 스카이 박스용

	ID3D11BlendState*			pDefaultBlendState;			// 기본 블렌드 상태
	ID3D11RasterizerState*		pDefaultRRState;			// 기본 레스터화 규칙

	D3D11_VIEWPORT				shadowViewPort{};			// Shadow 뷰포트
	ID3D11Texture2D*			pShadowMap[cb_PBRDirectionalLight::MAX_LIGHT_COUNT]{};		// Shadow 맵
	ID3D11DepthStencilView*		pShadowMapDSV[cb_PBRDirectionalLight::MAX_LIGHT_COUNT]{};	// Shadow Map용 DSV
	ID3D11ShaderResourceView*	pShadowMapSRV[cb_PBRDirectionalLight::MAX_LIGHT_COUNT]{};	// Shadow Map용 SRV
	DirectX::SimpleMath::Matrix	shadowViews[cb_PBRDirectionalLight::MAX_LIGHT_COUNT]{};
	DirectX::SimpleMath::Matrix	shadowProjections[cb_PBRDirectionalLight::MAX_LIGHT_COUNT]{};
	ID3D11InputLayout*			pShadowInputLayout;			
	ID3D11VertexShader*			pShadowVertexShader;		
	ID3D11InputLayout*			pShadowSkinningInputLayout;		   
	ID3D11VertexShader*			pShadowSkinningVertexShader;		

private:
	std::unique_ptr<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>> pPrimitiveBatch;
	std::unique_ptr<DirectX::DX11::BasicEffect> pBasicEffect;
	std::vector<std::tuple<const DirectX::SimpleMath::Matrix*, const DirectX::SimpleMath::Matrix*>> debugFrustumVec;
	void DrawDebug();

private:
	DirectX::SimpleMath::Matrix cullingIVM;	   //카메라 월드
	DirectX::SimpleMath::Matrix cullingView;	   //카메라 뷰
	DirectX::SimpleMath::Matrix cullingProjection; //카메라 투영

public:
	bool DebugDrawLightFrustum = false;
	bool DebugDrawObjectCullingBox = false;
	bool DebugDrawCameraFrustum = false;
	bool DebugLockCameraFrustum = false;

	void PushDebugFrustum(const DirectX::SimpleMath::Matrix* frustum, const DirectX::SimpleMath::Matrix* WM);
	void PopDebugFrustum();
	bool CheckFrustumCulling(GameObject* obj) const;
public:
	size_t GetDrawCount() const { return DrawCallCount; }

private:
	std::vector<RENDERER_DRAW_DESC> opaquerenderOueue; //불투명 오브젝트
	std::vector<RENDERER_DRAW_DESC> alphaRenderQueue;  //반투명 오브젝트
	size_t DrawCallCount = 0;
	void RenderSkyBox(class SkyBoxRender* skyBox);
	void RenderShadowMap(RENDERER_DRAW_DESC& drawDesc);	  
	void RenderScene(RENDERER_DRAW_DESC& drawDesc);
	void CreateRTV();
public:
	//현재 디스플레이 설정 가져오기.
	DXGI_MODE_DESC1 GetDisplayMode(int AdapterIndex, int OutputIndex);

	//가장 가까운 해상도의 모드 가져오기.
	DXGI_MODE_DESC1 GetClosestResolution(int AdapterIndex, int OutputIndex, SIZE resolution);

	//사용 가능한 화면 모드들 가져오기.
	std::vector<DXGI_MODE_DESC1> GetDisplayModeList(int AdapterIndex, int OutputIndex);

	//스왑 체인 재생성
	void ReCreateSwapChain(DXGI_SWAP_CHAIN_DESC1* swapChainDesc); 

	//전체화면 <-> 창모드 전환.
	void ToggleFullscreenMode();

	bool IsSwapChainWindowed() const { return swapChainWindowed; }

private:
	bool swapChainWindowed = false;
	const wchar_t* GetDisplayRotationToCWStr(DXGI_MODE_ROTATION rotation);

	DWORD GetColorBitDepth(DXGI_FORMAT format);
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
	bool isSkinning;
	bool isAlpha;
};


