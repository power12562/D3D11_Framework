#pragma once
#include <Utility\D3D11Utility.h> //helper
#include <Utility\MemoryUtility.h>
#include <Core/TSingleton.h>
#include <map>
#include <string>
#include <cassert>
#include <Light/PBRDirectionalLight.h>
#include <D3DCore/D3DSamplerState.h>

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
	//���� ����ȭ ��� ����
	bool UseVSync = false;
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
	ID3D11ShaderResourceView*	GetGbufferSRV(int index) { return pGbufferSRV[index]; }
	ID3D11DepthStencilView*		GetDepthStencilView() { return pDepthStencilView; }
	ID3D11ShaderResourceView*	GetShadowMapSRV(int index) { return pShadowMapSRV[index]; }

	/** Vram ��뷮 Ȯ�ο�.*/
	USAGE_VRAM_INFO GetLocalVramUsage();
	/** Vram ��뷮 Ȯ�ο�.*/
	USAGE_VRAM_INFO GetNonLocalVramUsage();
	/** gpu �޸� ���� Ȯ�ο�*/
	SYSTEM_VRAM_INFO GetSystemVramInfo();
	/** �ý��� �޸� ���� Ȯ�ο�*/
	SYSTEM_MEMORY_INFO GetSystemMemoryInfo();

	void reserveRenderQueue(size_t size); 

	/**�⺻ ���·� OM ���� ���� */
	void SetDefaultOMState();

	/*������ȭ �⺻ ��Ģ ����*/
	void SetRRState(D3D11_RASTERIZER_DESC& defaultDesc);

	/*������ȭ ��Ģ ����*/
	void CreateRRState(D3D11_RASTERIZER_DESC& RASTERIZER_DESC, ID3D11RasterizerState** rasterState);

private:
	//Vram üũ�� dxgi ��ü
	IDXGIFactory4* pDXGIFactory = nullptr;
	std::vector<IDXGIAdapter3*> DXGIAdapters;
	std::vector<std::vector<IDXGIOutput1*>> DXGIOutputs;

public:
	void SetRTVdefault();
	void ClearRTV();
	void BegineDraw();
	void DrawIndex(RENDERER_DRAW_DESC& darwDesc);
	void EndDraw();
	void Present();

public:
	// ViewPorts
	std::vector<D3D11_VIEWPORT> ViewPortsVec;			
	D3D11_VIEWPORT GetClientSizeViewport();

	//Clear Color
	DirectX::SimpleMath::Color backgroundColor;

	//Setting
	RENDERER_SETTING_DESC setting;
private:
	ID3D11Device*				pDevice;					// ����̽�	
	ID3D11DeviceContext*		pDeviceContext;				// ����̽� ���ؽ�Ʈ
	IDXGISwapChain1*			pSwapChain;					// ����ü��
	ID3D11RenderTargetView*		pRenderTargetViewArray[8]{};// RTV Array
	ID3D11ShaderResourceView*	pGbufferSRV[7]{};			// Gbuffer SRV Array
	ID3D11DepthStencilView*     pGbufferDSV;				// Gbuffer DSV;

	ID3D11DepthStencilView*		pDepthStencilView;			// ���� ����/���ٽ� ����
	ID3D11DepthStencilState*	pDefaultDepthStencilState;  // �⺻ ����

	ID3D11BlendState*			pDefaultBlendState;			// �⺻ ���� ����
	ID3D11RasterizerState*		pDefaultRRState;			// �⺻ ������ȭ ��Ģ

	D3D11_VIEWPORT				shadowViewPort{};			// Shadow ����Ʈ
	ID3D11Texture2D*			pShadowMap[cb_PBRDirectionalLight::MAX_LIGHT_COUNT]{};		// Shadow ��
	ID3D11DepthStencilView*		pShadowMapDSV[cb_PBRDirectionalLight::MAX_LIGHT_COUNT]{};	// Shadow Map�� DSV
	ID3D11ShaderResourceView*	pShadowMapSRV[cb_PBRDirectionalLight::MAX_LIGHT_COUNT]{};	// Shadow Map�� SRV
	DirectX::SimpleMath::Matrix	shadowViews[cb_PBRDirectionalLight::MAX_LIGHT_COUNT]{};
	DirectX::SimpleMath::Matrix	shadowProjections[cb_PBRDirectionalLight::MAX_LIGHT_COUNT]{};
	ID3D11InputLayout*			pShadowInputLayout;			
	ID3D11VertexShader*			pShadowVertexShader;		
	ID3D11InputLayout*			pShadowSkinningInputLayout;		   
	ID3D11VertexShader*			pShadowSkinningVertexShader;		
private:
	void CreateShadowMapResource();

private:
	std::unique_ptr<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>> pPrimitiveBatch;
	std::unique_ptr<DirectX::DX11::BasicEffect> pBasicEffect;
	std::vector<std::tuple<const DirectX::SimpleMath::Matrix*, const DirectX::SimpleMath::Matrix*>> debugFrustumVec;
	void DrawDebug();

private:
	DirectX::SimpleMath::Matrix cullingIVM;		   
	DirectX::SimpleMath::Matrix cullingView;	   //ī�޶� ��		
	DirectX::SimpleMath::Matrix cullingProjection; //ī�޶� ����

public:
	//Gizmo Color
	DirectX::XMVECTORF32 debugDrawColor;

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
	inline static const int GbufferCount = 4;
	std::vector<RENDERER_DRAW_DESC> opaquerenderOueue;  //���۵� ������ ������Ʈ
	std::vector<RENDERER_DRAW_DESC> forwardrenderOueue; //������ ������Ʈ ������
	std::vector<RENDERER_DRAW_DESC> alphaRenderQueue;   //������ ������Ʈ
	size_t DrawCallCount = 0;
	void RenderSkyBox(class SkyBoxRender* skyBox);
	void RenderShadowMap(RENDERER_DRAW_DESC& drawDesc);	  

	//Deferred Scene
	void RenderSceneGbuffer(RENDERER_DRAW_DESC& drawDesc);
	void RenderSceneLight();

	//Forward Scene
	void RenderSceneForward(RENDERER_DRAW_DESC& drawDesc);

public:
	void CreateDeferredResource();
private:
	ID3D11PixelShader*		pPBRDirectionalLightPS  = nullptr;
	ID3D11VertexShader*		pDeferredVertexShader  = nullptr;
	ID3D11InputLayout*		pDeferredInputLayout  = nullptr;
	DRAW_INDEX_DATA			deferredDrawData{};
	std::unique_ptr<D3DSamplerState> deferredSamplers{};

	void CreateMainDSV();
	void CreateGbufferRTV();
	void CreateGbufferDSVnSRV();
public:
	//���� ���÷��� ���� ��������.
	DXGI_MODE_DESC1 GetDisplayMode(int AdapterIndex, int OutputIndex);

	//���� ����� �ػ��� ��� ��������.
	DXGI_MODE_DESC1 GetClosestResolution(int AdapterIndex, int OutputIndex, SIZE resolution);

	//��� ������ ȭ�� ���� ��������.
	std::vector<DXGI_MODE_DESC1> GetDisplayModeList(int AdapterIndex, int OutputIndex);

	//���� ü�� �����
	void ReCreateSwapChain(DXGI_SWAP_CHAIN_DESC1* swapChainDesc); 

	//��üȭ�� <-> â��� ��ȯ.
	void ToggleFullscreenMode();

	bool IsSwapChainWindowed() const { return swapChainWindowed; }

private:
	bool swapChainWindowed = false;
	const wchar_t* GetDisplayRotationToCWStr(DXGI_MODE_ROTATION rotation);

	DWORD GetColorBitDepth(DXGI_FORMAT format);
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

enum RENDER_FALG
{
	RENDER_SKINNING = 1 << 0,
	RENDER_ALPHA    = 1 << 1,
	RENDER_FORWARD  = 1 << 2,
};

struct RENDERER_DRAW_DESC 
{
	using RenderFlags = int;

	DRAW_INDEX_DATA* pVertexIndex;
	D3DConstBuffer*  pConstBuffer;
	D3DTexture2D*	 pD3DTexture2D;
	D3DSamplerState* pSamperState;
	const Transform* pTransform;
	ID3D11InputLayout* pInputLayout;
	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3D11RasterizerState* pRRState = nullptr;
	RenderFlags flags;
};


