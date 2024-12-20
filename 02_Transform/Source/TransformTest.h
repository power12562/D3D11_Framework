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
	// ������ ������������ �����ϴ� �ʼ� ��ü�� �������̽�(�X�� ���ٽ� �䵵 ������ ���� ������� �ʴ´�.)
	inline static struct ID3D11Device* pDevice = nullptr;						// ����̽�	
	inline static struct ID3D11DeviceContext* pDeviceContext = nullptr;		// ��� ����̽� ���ؽ�Ʈ
	inline static struct IDXGISwapChain* pSwapChain = nullptr;					// ����ü��
	inline static struct ID3D11RenderTargetView* pRenderTargetView = nullptr;	// ������ Ÿ�ٺ�
	inline static struct ID3D11DepthStencilView* pDepthStencilView = nullptr;

public:
	static ID3D11Device* GetDevice() { return pDevice; }
	static ID3D11DeviceContext* GetDeviceContext() { return pDeviceContext; }
	static ID3D11RenderTargetView* GetRenderTargetView() { return pRenderTargetView; }
	static ID3D11DepthStencilView* GetDepthStencilView() { return pDepthStencilView; }

	// ������ ���������ο� �����ϴ�  ��ü�� ����
	struct ID3D11VertexShader* m_pVertexShader = nullptr;	// ���� ���̴�.
	struct ID3D11PixelShader* m_pPixelShader = nullptr;	// �ȼ� ���̴�.	
	struct ID3D11InputLayout* m_pInputLayout = nullptr;	// �Է� ���̾ƿ�.
	struct ID3D11Buffer* m_pConstantBuffer = nullptr;		// ��� ����.

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

