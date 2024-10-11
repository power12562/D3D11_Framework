#pragma once
#include <Utility\D3D11Utility.h> //helper
#include <Utility/MemoryUtility.h>
#include <Framework\TSingleton.h>
#include <map>
#include <string>
#include <cassert>

class D3DRenderer;
extern D3DRenderer& d3dRenderer;

/* b0 레지스터는 Transform 버퍼로 고정 사용.*/
struct cbuffer_Transform
{
	DirectX::SimpleMath::Matrix World;
	DirectX::SimpleMath::Matrix WorldInverseTranspose;
	DirectX::SimpleMath::Matrix WVP;
};

/* b1 레지스터는 Camera 버퍼로 고정 사용.*/
struct cbuffer_Camera 
{
	DirectX::SimpleMath::Matrix View;
	DirectX::SimpleMath::Matrix Projection;
};

/*copy to fxh file

cbuffer cbuffer_Transform : register(b0)
{
	Matrix World;
	Matrix WorldInverseTranspose;
	Matrix WVP;
};

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
	
public:
	struct REG_INDEX
	{
		int vs_index;
		int ps_index;
	};

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
	/*
	VS, PS 상수 버퍼 생성.
	return : register index
	*/
	template<typename T>
	REG_INDEX CreateVSPSConstantBuffers();
	REG_INDEX CreateVSPSConstantBuffers(const char* key, unsigned int buffer_size);

	/*
	VS 상수 버퍼 생성.
	return : register index
	*/
	template<typename T>
	int CreateVSConstantBuffers();
	int CreateVSConstantBuffers(const char* key, unsigned int buffer_size);

	/*
	PS 상수 버퍼 생성.
	return : register index
	*/
	template<typename T>
	int CreatePSConstantBuffers();
	int CreatePSConstantBuffers(const char* key, unsigned int buffer_size);
								   
	template<typename T>
	void UpdateVSPSConstBuffer(T& data);

	template<typename T>
	void UpdateVSConstBuffer(T& data);

	template<typename T>
	void UpdatePSConstBuffer(T& data);

	template<typename T>
	int GetVSConstBufferIndex();

	template<typename T>
	int GetPSConstBufferIndex();

public:
	void BegineDraw();
	void EndDraw();

public:
	void DrawIndex(DRAW_INDEX_DATA& data);

public:
	DirectX::SimpleMath::Color backgroundColor{ 0,0,0,1 };

private:
	ID3D11Device*			 pDevice;			  // 디바이스	
	ID3D11DeviceContext*	 pDeviceContext;	  // 디바이스 컨텍스트
	IDXGISwapChain*			 pSwapChain;		  // 스왑체인
	ID3D11RenderTargetView*  pRenderTargetView;	  // 렌더링 타겟뷰
	ID3D11DepthStencilView*  pDepthStencilView;   // 깊이 버퍼
	ID3D11BlendState*		 pBlendState;		  // 블렌드 상태

public:
	int GetVSCbufferCount() { return (int)vs_cbufferList.size(); }
	int GetPSCbufferCount() { return (int)ps_cbufferList.size(); }

private:
	std::map<std::string, int> vs_cbufferMap;
	std::vector<ID3D11Buffer*> vs_cbufferList;

	std::map<std::string, int> ps_cbufferMap;
	std::vector<ID3D11Buffer*> ps_cbufferList;

private:
	void SetConstBuffer();						// VS, PS에 상수버퍼 바인딩

};

template<typename T>
inline D3DRenderer::REG_INDEX D3DRenderer::CreateVSPSConstantBuffers()
{
	REG_INDEX index{};
	index.vs_index = CreateVSConstantBuffers<T>();
	index.ps_index = CreatePSConstantBuffers<T>();
	return index;
}

template<typename T>
inline int D3DRenderer::CreateVSConstantBuffers()
{
	static_assert((sizeof(T) % 16) == 0, "Constant Buffer size must be 16 - byte aligned");

	//상수 버퍼 생성
	std::string key = typeid(T).name();
	size_t temp = key.find(" ");
	if (temp != std::string::npos)
		key = key.substr(temp + 1);
	else
	{
		__debugbreak(); //키값 이상함.
		return -1;
	}
	return CreateVSConstantBuffers(key.c_str(), sizeof(T));
}

template<typename T>
inline int D3DRenderer::CreatePSConstantBuffers()
{
	static_assert((sizeof(T) % 16) == 0, "Constant Buffer size must be 16 - byte aligned");

	//상수 버퍼 생성
	std::string key = typeid(T).name();
	size_t temp = key.find(" ");
	if (temp != std::string::npos)
		key = key.substr(temp + 1);
	else
	{
		__debugbreak(); //키값 이상함.
		return -1;
	}

	return CreatePSConstantBuffers(key.c_str(), sizeof(T));
}

template<typename T>
inline void D3DRenderer::UpdateVSPSConstBuffer(T& data)
{
	UpdateVSConstBuffer<T>(data);
	UpdatePSConstBuffer<T>(data);
}

template<typename T>
inline void D3DRenderer::UpdateVSConstBuffer(T& data)
{
	std::string key = typeid(T).name();
	size_t temp = key.find(" ");
	if (temp != std::string::npos)
		key = key.substr(temp + 1);
	else
	{
		__debugbreak(); //키값 이상함.
		return;
	}

	auto findIter = vs_cbufferMap.find(key);
	if (findIter != vs_cbufferMap.end())
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Utility::CheackHRESULT(pDeviceContext->Map(vs_cbufferList[findIter->second], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		// 상수 버퍼에 데이터를 쓴다.
		T* dataPtr = (T*)mappedResource.pData;
		*dataPtr = data;

		// 맵핑 해제
		pDeviceContext->Unmap(vs_cbufferList[findIter->second], 0);
	}
	else
	{
		__debugbreak(); //존재하지 않는 키.
		return;
	}
}

template<typename T>
inline void D3DRenderer::UpdatePSConstBuffer(T& data)
{
	std::string key = typeid(T).name();
	size_t temp = key.find(" ");
	if (temp != std::string::npos)
		key = key.substr(temp + 1);
	else
	{
		__debugbreak(); //키값 이상함.
		return;
	}

	auto findIter = ps_cbufferMap.find(key);
	if (findIter != ps_cbufferMap.end())
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Utility::CheackHRESULT(pDeviceContext->Map(ps_cbufferList[findIter->second], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		// 상수 버퍼에 데이터를 쓴다.
		T* dataPtr = (T*)mappedResource.pData;
		*dataPtr = data;

		// 맵핑 해제
		pDeviceContext->Unmap(ps_cbufferList[findIter->second], 0);
	}
	else
	{
		__debugbreak(); //존재하지 않는 키.
		return;
	}
}

template<typename T>
inline int D3DRenderer::GetVSConstBufferIndex()
{
	std::string key = typeid(T).name();
	size_t temp = key.find(" ");
	if (temp != std::string::npos)
		key = key.substr(temp + 1);
	else
	{
		__debugbreak(); //키값 이상함.
		return -1;
	}

	auto findIter = vs_cbufferMap.find(key);
	if (findIter != vs_cbufferMap.end())
	{
		return 	findIter->second;
	}

	return -1;
}

template<typename T>
inline int D3DRenderer::GetPSConstBufferIndex()
{
	std::string key = typeid(T).name();
	size_t temp = key.find(" ");
	if (temp != std::string::npos)
		key = key.substr(temp + 1);
	else
	{
		__debugbreak(); //키값 이상함.
		return -1;
	}

	auto findIter = ps_cbufferMap.find(key);
	if (findIter != ps_cbufferMap.end())
	{
		return 	findIter->second;
	}

	return -1;
}