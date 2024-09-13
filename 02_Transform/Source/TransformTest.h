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
	// ������ ������������ �����ϴ� �ʼ� ��ü�� �������̽�(�X�� ���ٽ� �䵵 ������ ���� ������� �ʴ´�.)
	inline static struct ID3D11Device* m_pDevice = nullptr;						// ����̽�	
	inline static struct ID3D11DeviceContext* m_pDeviceContext = nullptr;		// ��� ����̽� ���ؽ�Ʈ
	inline static struct IDXGISwapChain* m_pSwapChain = nullptr;					// ����ü��
	inline static struct ID3D11RenderTargetView* m_pRenderTargetView = nullptr;	// ������ Ÿ�ٺ�

public:
	static ID3D11Device* GetDevice() { return m_pDevice; }
	static ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext; }

	// ������ ���������ο� �����ϴ�  ��ü�� ����
	struct ID3D11VertexShader* m_pVertexShader = nullptr;	// ���� ���̴�.
	struct ID3D11PixelShader* m_pPixelShader = nullptr;	// �ȼ� ���̴�.	
	struct ID3D11InputLayout* m_pInputLayout = nullptr;	// �Է� ���̾ƿ�.
	struct ID3D11Buffer* m_pConstantBuffer = nullptr;		// ��� ����.

protected:
	virtual void Start();
	virtual void Update();
	virtual void Render();

private:
	std::vector<SimpleObject*> objList{};

private:
	void ClearObjList();

};