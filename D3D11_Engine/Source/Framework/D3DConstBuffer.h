#pragma once
#include <Framework\D3DRenderer.h>

/* b0 �������ʹ� Transform ���۷� ���� ���.*/
struct cb_Transform
{
	DirectX::SimpleMath::Matrix World;
	DirectX::SimpleMath::Matrix WorldInverseTranspose;
	DirectX::SimpleMath::Matrix WVP;
};

/* b1 �������ʹ� Camera ���۷� ���� ���.*/
struct cb_Camera
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

class D3DConstBuffer
{
	friend D3DRenderer;
public:
	struct REG_INDEX
	{
		int vs_index;
		int ps_index;
	};
public:
	D3DConstBuffer();
	~D3DConstBuffer();

private:
	inline static cb_Transform TransformBuffer{};
	inline static cb_Camera CameraBuffer{};

public:
	/*
	VS, PS ��� ���� ����.
	return : register index
	*/
	template<typename T>
	REG_INDEX CreateVSPSConstantBuffers();
	REG_INDEX CreateVSPSConstantBuffers(const char* key, unsigned int buffer_size);

	/*
	VS ��� ���� ����.
	return : register index
	*/
	template<typename T>
	int CreateVSConstantBuffers();
	int CreateVSConstantBuffers(const char* key, unsigned int buffer_size);

	/*
	PS ��� ���� ����.
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

private:
	std::map<std::string, int> vs_cbufferMap;
	std::vector<ID3D11Buffer*> vs_cbufferList;

	std::map<std::string, int> ps_cbufferMap;
	std::vector<ID3D11Buffer*> ps_cbufferList;

public:
	int GetVSCbufferCount() { return (int)vs_cbufferList.size(); }
	int GetPSCbufferCount() { return (int)ps_cbufferList.size(); }

};

template<typename T>
inline D3DConstBuffer::REG_INDEX D3DConstBuffer::CreateVSPSConstantBuffers()
{
	REG_INDEX index{};
	index.vs_index = CreateVSConstantBuffers<T>();
	index.ps_index = CreatePSConstantBuffers<T>();
	return index;
}

template<typename T>
inline int D3DConstBuffer::CreateVSConstantBuffers()
{
	static_assert((sizeof(T) % 16) == 0, "Constant Buffer size must be 16 - byte aligned");

	//��� ���� ����
	std::string key = typeid(T).name();
	size_t temp = key.find(" ");
	if (temp != std::string::npos)
		key = key.substr(temp + 1);
	else
	{
		__debugbreak(); //Ű�� �̻���.
		return -1;
	}
	return CreateVSConstantBuffers(key.c_str(), sizeof(T));
}

template<typename T>
inline int D3DConstBuffer::CreatePSConstantBuffers()
{
	static_assert((sizeof(T) % 16) == 0, "Constant Buffer size must be 16 - byte aligned");

	//��� ���� ����
	std::string key = typeid(T).name();
	size_t temp = key.find(" ");
	if (temp != std::string::npos)
		key = key.substr(temp + 1);
	else
	{
		__debugbreak(); //Ű�� �̻���.
		return -1;
	}

	return CreatePSConstantBuffers(key.c_str(), sizeof(T));
}

template<typename T>
inline void D3DConstBuffer::UpdateVSPSConstBuffer(T& data)
{
	UpdateVSConstBuffer<T>(data);
	UpdatePSConstBuffer<T>(data);
}

template<typename T>
inline void D3DConstBuffer::UpdateVSConstBuffer(T& data)
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

	auto findIter = vs_cbufferMap.find(key);
	if (findIter != vs_cbufferMap.end())
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Utility::CheackHRESULT(d3dRenderer.GetDeviceContext()->Map(vs_cbufferList[findIter->second], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		// ��� ���ۿ� �����͸� ����.
		T* dataPtr = (T*)mappedResource.pData;
		*dataPtr = data;

		// ���� ����
		d3dRenderer.GetDeviceContext()->Unmap(vs_cbufferList[findIter->second], 0);
	}
	else
	{
		__debugbreak(); //�������� �ʴ� Ű.
		return;
	}
}

template<typename T>
inline void D3DConstBuffer::UpdatePSConstBuffer(T& data)
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

	auto findIter = ps_cbufferMap.find(key);
	if (findIter != ps_cbufferMap.end())
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Utility::CheackHRESULT(d3dRenderer.GetDeviceContext()->Map(ps_cbufferList[findIter->second], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		// ��� ���ۿ� �����͸� ����.
		T* dataPtr = (T*)mappedResource.pData;
		*dataPtr = data;

		// ���� ����
		d3dRenderer.GetDeviceContext()->Unmap(ps_cbufferList[findIter->second], 0);
	}
	else
	{
		__debugbreak(); //�������� �ʴ� Ű.
		return;
	}
}

template<typename T>
inline int D3DConstBuffer::GetVSConstBufferIndex()
{
	std::string key = typeid(T).name();
	size_t temp = key.find(" ");
	if (temp != std::string::npos)
		key = key.substr(temp + 1);
	else
	{
		__debugbreak(); //Ű�� �̻���.
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
inline int D3DConstBuffer::GetPSConstBufferIndex()
{
	std::string key = typeid(T).name();
	size_t temp = key.find(" ");
	if (temp != std::string::npos)
		key = key.substr(temp + 1);
	else
	{
		__debugbreak(); //Ű�� �̻���.
		return -1;
	}

	auto findIter = ps_cbufferMap.find(key);
	if (findIter != ps_cbufferMap.end())
	{
		return 	findIter->second;
	}

	return -1;
}