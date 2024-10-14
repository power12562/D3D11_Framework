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

cbuffer cb_Transform : register(b0)
{
	Matrix World;
	Matrix WorldInverseTranspose;
	Matrix WVP;
};

cbuffer cb_Camera : register(b1)
{
	Matrix View;
	Matrix Projection;
};

*/

class D3DConstBuffer
{
	friend D3DRenderer;
public:
	static void CreateStaticCbuffer();
	static void ReleaseStaticCbuffer();

	template<typename T>
	static void UpdateStaticCbuffer(const T& data);
	template<>
	static void UpdateStaticCbuffer(const cb_Transform& data);
	template<>
	static void UpdateStaticCbuffer(const cb_Camera& data);

public:
	D3DConstBuffer();
	~D3DConstBuffer();

private:
	inline static constexpr int StaticCbufferCount = 2;
	inline static ID3D11Buffer* cBufferTransform = nullptr;
	inline static ID3D11Buffer* cBufferCamera = nullptr;

private:
	inline static std::unordered_map<std::string, ID3D11Buffer*> cBufferMap;
public:
	void SetConstBuffer(); //��� ���� ���ε�

	/*
	VS ��� ���� ����.
	return : register index
	*/
	template<typename T>
	int CreateVSConstantBuffers();
	/*
	PS ��� ���� ����.
	return : register index
	*/
	template<typename T>
	int CreatePSConstantBuffers();

	template<typename T>
	void UpdateConstBuffer(T& data);

	template<typename T>
	int GetVSConstBufferIndex();
	template<typename T>
	int GetPSConstBufferIndex();

private:
	std::vector<std::string> vs_cbufferList{};
	std::vector<std::string> ps_cbufferList{};

public:
	int GetVSCbufferCount() { return (int)vs_cbufferList.size() + StaticCbufferCount; }
	int GetPSCbufferCount() { return (int)ps_cbufferList.size() + StaticCbufferCount; }

};

template<typename T>
inline void D3DConstBuffer::UpdateStaticCbuffer(const T& data)
{
	static_assert(false, "T is not static buffer");
}

template<>
inline void D3DConstBuffer::UpdateStaticCbuffer(const cb_Transform& data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Utility::CheackHRESULT(d3dRenderer.GetDeviceContext()->Map(cBufferTransform, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	// ��� ���ۿ� �����͸� ����.
	cb_Transform* dataPtr = (cb_Transform*)mappedResource.pData;
	*dataPtr = data;

	// ���� ����
	d3dRenderer.GetDeviceContext()->Unmap(cBufferTransform, 0);
}

template<>
inline void D3DConstBuffer::UpdateStaticCbuffer(const cb_Camera& data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Utility::CheackHRESULT(d3dRenderer.GetDeviceContext()->Map(cBufferCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	// ��� ���ۿ� �����͸� ����.
	cb_Camera* dataPtr = (cb_Camera*)mappedResource.pData;
	*dataPtr = data;

	// ���� ����
	d3dRenderer.GetDeviceContext()->Unmap(cBufferCamera, 0);
}

template<typename T>
inline int D3DConstBuffer::CreateVSConstantBuffers()
{
	static_assert((sizeof(T) % 16) == 0, "Constant Buffer size must be 16 - byte aligned");

	//��� ���� ����
	std::string key = typeid(T).name();

	if (GetVSCbufferCount() == D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
	{
		__debugbreak(); //��� ���� �ִ� ���� ����.
		return -1;
	}

	int regIndex = GetVSCbufferCount();
	if (cBufferMap.find(key) == cBufferMap.end()) //Ű�� ��ã���� ���ҽ� ����
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // ��� ���۴� dynamic���� �����ϰ� ���� ����.
		bufferDesc.ByteWidth = sizeof(T);  // ��� ���� ũ��
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Buffer* cBufferTemp{};
		Utility::CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferTemp));
		cBufferMap[key] = cBufferTemp;
	}
	vs_cbufferList.emplace_back(key);
	return regIndex;
}

template<typename T>
inline int D3DConstBuffer::CreatePSConstantBuffers()
{
	static_assert((sizeof(T) % 16) == 0, "Constant Buffer size must be 16 - byte aligned");

	//��� ���� ����
	std::string key = typeid(T).name();

	if (GetPSCbufferCount() == D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
	{
		__debugbreak(); //��� ���� �ִ� ���� ����.
		return -1;
	}

	int regIndex = GetPSCbufferCount();
	if (cBufferMap.find(key) == cBufferMap.end())
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // ��� ���۴� dynamic���� �����ϰ� ���� ����.
		bufferDesc.ByteWidth = sizeof(T);  // ��� ���� ũ��
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Buffer* cBufferTemp{};
		Utility::CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferTemp));
		cBufferMap[key] = cBufferTemp;
	}
	ps_cbufferList.emplace_back(key);
	return regIndex;
}

template<typename T>
inline void D3DConstBuffer::UpdateConstBuffer(T& data)
{
	std::string key = typeid(T).name();

	auto findIter = cBufferMap.find(key);
	if (findIter != cBufferMap.end())
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Utility::CheackHRESULT(d3dRenderer.GetDeviceContext()->Map(findIter->second, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		// ��� ���ۿ� �����͸� ����.
		T* dataPtr = (T*)mappedResource.pData;
		*dataPtr = data;

		// ���� ����
		d3dRenderer.GetDeviceContext()->Unmap(findIter->second, 0);
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

	int index = StaticCbufferCount;
	for (auto& item : vs_cbufferList)
	{
		if (key == item)
			return index;	
		index++;
	}
	return -1;
}

template<typename T>
inline int D3DConstBuffer::GetPSConstBufferIndex()
{
	std::string key = typeid(T).name();

	int index = StaticCbufferCount;
	for (auto& item : ps_cbufferList)
	{
		if (key == item)
			return index;
		index++;
	}
	return -1;
}