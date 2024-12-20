#pragma once
#include <Core/WinGameApp.h>
#include <vector>

class SimpleObject;
class TransformTest : public WinGameApp
{
public:
	TransformTest();
	~TransformTest();

	bool InitD3D();
	void UninitD3D();

	bool InitScene();
	void UninitScene();

	virtual void InitImGUI() override;
public:
	// 렌더링 파이프라인을 구성하는 필수 객체의 인터페이스(뎊스 스텐실 뷰도 있지만 아직 사용하지 않는다.)
	inline static struct ID3D11Device* pDevice = nullptr;						// 디바이스	
	inline static struct ID3D11DeviceContext* pDeviceContext = nullptr;		// 즉시 디바이스 컨텍스트
	inline static struct IDXGISwapChain* pSwapChain = nullptr;					// 스왑체인
	inline static struct ID3D11RenderTargetView* pRenderTargetView = nullptr;	// 렌더링 타겟뷰
	inline static struct ID3D11DepthStencilView* pDepthStencilView = nullptr;

public:
	static ID3D11Device* GetDevice() { return pDevice; }
	static ID3D11DeviceContext* GetDeviceContext() { return pDeviceContext; }
	static ID3D11RenderTargetView* GetRenderTargetView() { return pRenderTargetView; }
	static ID3D11DepthStencilView* GetDepthStencilView() { return pDepthStencilView; }

	// 렌더링 파이프라인에 적용하는  객체와 정보
	struct ID3D11VertexShader* m_pVertexShader = nullptr;	// 정점 셰이더.
	struct ID3D11PixelShader* m_pPixelShader = nullptr;	// 픽셀 셰이더.	
	struct ID3D11InputLayout* m_pInputLayout = nullptr;	// 입력 레이아웃.
	struct ID3D11Buffer* m_pConstantBuffer = nullptr;		// 상수 버퍼.

protected:
	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	void ImGUIRender();

private:
	std::vector<SimpleObject*> objList{};

private:
	void ClearObjList();

};

