#pragma once
#include <Utility\D3D11Utility.h> //helper
#include <Utility/MemoryUtility.h>
#include <Framework\TSingleton.h>
#include <map>
#include <string>
#include <cassert>

class D3DRenderer;
extern D3DRenderer& d3dRenderer;

/* b0 �������ʹ� Transform ���۷� ���� ���.*/
struct cbuffer_Transform
{
	DirectX::SimpleMath::Matrix World;
	DirectX::SimpleMath::Matrix WVP;
};

/* b1 �������ʹ� Camera ���۷� ���� ���.*/
struct cbuffer_Camera
{
	DirectX::SimpleMath::Matrix View;
	DirectX::SimpleMath::Matrix Projection;
};

/*copy to fxh file

cbuffer cbuffer_Transform : register(b0)
{
	Matrix World;
	Matrix WVP;
}

cbuffer cbuffer_Camera : register(b1)
{
	Matrix View;
	Matrix Projection;
};

*/

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
	template<typename T>
	void CreateConstantBuffers();

	template<typename T>
	void UpdateConstBuffer(T& data);

public:
	void BegineDraw();
	void EndDraw();

public:
	void DrawIndex(DRAW_INDEX_DATA& data);

public:
	DirectX::SimpleMath::Color backgroundColor{ 0,0,0,1 };

private:
	ID3D11Device*			 pDevice;			  // ����̽�	
	ID3D11DeviceContext*	 pDeviceContext;	  // ����̽� ���ؽ�Ʈ
	IDXGISwapChain*			 pSwapChain;		  // ����ü��
	ID3D11RenderTargetView*  pRenderTargetView;	  // ������ Ÿ�ٺ�
	ID3D11DepthStencilView*  pDepthStencilView;   // ���� ����

private:
	std::map<std::string, int> cbufferMap;		// ��� ���� ���. �̸� �˻���
	std::vector<ID3D11Buffer*> cbufferList;		// ��� ���� ���. �������� �ε��� ���ٿ�.

private:
	void SetConstBuffer();						// VS, PS�� ������� ���ε�

};

template<typename T>
inline void D3DRenderer::CreateConstantBuffers()
{
	static_assert((sizeof(T) % 16) == 0, "Constant Buffer size must be 16 - byte aligned");
	if (cbufferList.size() == D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
	{
		__debugbreak(); //��� ���� �ִ� ���� ����.
		return;
	}

	//��� ���� ����
	std::string key = typeid(T).name();
	size_t temp = key.find(" ");
	if (temp != std::string::npos)
		key = key.substr(temp + 1);
	else
	{
		__debugbreak(); //Ű�� �̻���.
		return;
	}

	if (cbufferMap.find(key) != cbufferMap.end())
	{
		__debugbreak(); //�̹� �����ϴ� Ű��.
		return;
	}

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // ��� ���۴� dynamic���� �����ϰ� ���� ����.
	bufferDesc.ByteWidth = sizeof(T);  // ��� ���� ũ��
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		
	cbufferMap[key] = (int)cbufferList.size();
	ID3D11Buffer* cBufferTemp{};
	Utility::CheackHRESULT(pDevice->CreateBuffer(&bufferDesc, nullptr, &cBufferTemp));
	cbufferList.push_back(cBufferTemp);
}

template<typename T>
inline void D3DRenderer::UpdateConstBuffer(T& data)
{
	std::string key = typeid(T).name();
	size_t temp = key.find(" ");
	if (temp != std::string::npos)
		key = key.substr(temp + 1);
	else
	{
		__debugbreak(); //Ű�� �̻���.
		return;
	}

	auto findIter = cbufferMap.find(key);
	if (findIter != cbufferMap.end())
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Utility::CheackHRESULT(pDeviceContext->Map(cbufferList[findIter->second], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		// ��� ���ۿ� �����͸� ����.
		T* dataPtr = (T*)mappedResource.pData;
		*dataPtr = data;

		// ���� ����
		pDeviceContext->Unmap(cbufferList[findIter->second], 0);
	}
	else
	{
		__debugbreak(); //�������� �ʴ� Ű.
		return;
	}
}
