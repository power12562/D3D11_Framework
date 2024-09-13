#pragma once
#include <Framework/WinGameApp.h>
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

public:
	// 렌더링 파이프라인을 구성하는 필수 객체의 인터페이스(뎊스 스텐실 뷰도 있지만 아직 사용하지 않는다.)
	inline static struct ID3D11Device* m_pDevice = nullptr;						// 디바이스	
	inline static struct ID3D11DeviceContext* m_pDeviceContext = nullptr;		// 즉시 디바이스 컨텍스트
	inline static struct IDXGISwapChain* m_pSwapChain = nullptr;					// 스왑체인
	inline static struct ID3D11RenderTargetView* m_pRenderTargetView = nullptr;	// 렌더링 타겟뷰

public:
	static ID3D11Device* GetDevice() { return m_pDevice; }
	static ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext; }

	// 렌더링 파이프라인에 적용하는  객체와 정보
	struct ID3D11VertexShader* m_pVertexShader = nullptr;	// 정점 셰이더.
	struct ID3D11PixelShader* m_pPixelShader = nullptr;	// 픽셀 셰이더.	
	struct ID3D11InputLayout* m_pInputLayout = nullptr;	// 입력 레이아웃.
	struct ID3D11Buffer* m_pConstantBuffer = nullptr;		// 상수 버퍼.

protected:
	virtual void Start();
	virtual void Update();
	virtual void Render();

private:
	std::vector<SimpleObject*> objList{};

private:
	void ClearObjList();

};