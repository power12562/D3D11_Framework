#pragma once
#include <D3DCore/D3DRenderer.h> 
#include <memory>
#include <unordered_set>

/* b0 레지스터는 Transform 버퍼*/
struct cb_Transform
{
	DirectX::SimpleMath::Matrix World;
	DirectX::SimpleMath::Matrix WorldInverseTranspose;
	DirectX::SimpleMath::Matrix WVP;
};

/* b1 레지스터는 Camera 버퍼*/
struct cb_Camera
{
	DirectX::SimpleMath::Vector3 MainCamPos;
	float camPad{};

	DirectX::SimpleMath::Matrix View;
	DirectX::SimpleMath::Matrix Projection;
	DirectX::SimpleMath::Matrix IVM;
	DirectX::SimpleMath::Matrix IPM;
};

/* b2 레지스터는 ShadowMap 버퍼*/
struct cb_ShadowMap
{
	DirectX::SimpleMath::Matrix ShadowProjections[cb_PBRDirectionalLight::MAX_LIGHT_COUNT];
	DirectX::SimpleMath::Matrix ShadowViews[cb_PBRDirectionalLight::MAX_LIGHT_COUNT];
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
	float3 MainCamPos;
	Matrix View;
	Matrix Projection;
	Matrix IVM;
	Matrix IPM;
};

cbuffer cb_ShadowMap : register(b2)
{
	Matrix ShadowProjections[MAX_LIGHT_COUNT];
	Matrix ShadowViews[MAX_LIGHT_COUNT];
};

*/

/*상수 버퍼용 4바이트 bool*/
struct alignas(4) CBbool
{
	int value;

	CBbool() { value = false; }
	CBbool(bool value) { this->value = (int)value; }

	bool& operator=(bool value) { return (bool&)this->value = (int)value; }
	bool* operator&() { return (bool*)&value; }

	operator bool() const { return value; }
	operator bool*() { return (bool*)&value; }
};

class D3DConstBuffer
{
	friend D3DRenderer;
public:
	static constexpr bool UPDATE_MODE_MAP = true;
	static void CreateStaticCbuffer();
	static void ReleaseStaticCbuffer();
	static void SetStaticCbuffer();
	/*상수버퍼 초기화 여부 리셋. Scene 소멸자에서 호출됩니다.*/
	inline static void ClearInitFlag() { initFlagSet.clear(); }

	template<typename T>
	static void UpdateStaticCbuffer(const T& data);

	template<typename T>
	inline static std::shared_ptr<T> GetData(const char* key);
private:
	static std::shared_ptr<char[]> GetData(size_t size_of, const char* key);
	inline static std::string make_key(size_t size_of, const char* key);
public:
	D3DConstBuffer();
	~D3DConstBuffer();

public:
	D3DConstBuffer(const D3DConstBuffer& rhs) = delete;
	D3DConstBuffer& operator=(const D3DConstBuffer& rhs) = delete;

private:
	inline static constexpr int StaticCbufferCount = 3;
	inline static ID3D11Buffer* cBufferTransform = nullptr;
	inline static ID3D11Buffer* cBufferCamera = nullptr;
	inline static ID3D11Buffer* cBufferShadowMap = nullptr;

private:
	inline static std::unordered_map<size_t, ID3D11Buffer*> cbufferMap;
	inline static std::unordered_map<std::string, std::weak_ptr<char[]>> dataMap;
	inline static std::unordered_set<std::string> initFlagSet;

public:
	void SetConstBuffer(); //상수 버퍼 바인딩

	/*
	VS 상수 버퍼 생성.
	return : register index
	*/
	template<typename T>
	int CreateVSConstantBuffers(const char* key);
	/*
	PS 상수 버퍼 생성.
	return : register index
	*/
	template<typename T>
	int CreatePSConstantBuffers(const char* key);

private:
	int CreateVSConstantBuffers(size_t size_of, const char* key);
	int CreatePSConstantBuffers(size_t size_of, const char* key);

private:
	std::vector<std::string> vs_keyList{};
	std::vector<std::pair<size_t, std::shared_ptr<char[]>>> vs_dataList{};
	std::vector<std::string> ps_keyList{};
	std::vector<std::pair<size_t, std::shared_ptr<char[]>>> ps_dataList{};

	inline void UpdateVSconstBuffer(int index);
	inline void UpdatePSconstBuffer(int index);
public:
	int GetVSCbufferCount() { return (int)vs_keyList.size() + StaticCbufferCount; }
	int GetPSCbufferCount() { return (int)ps_keyList.size() + StaticCbufferCount; }

};

template<typename T>
inline void D3DConstBuffer::UpdateStaticCbuffer(const T& data)
{
	static_assert(false, "T is not static buffer");
}

template<typename T>
inline std::shared_ptr<T> D3DConstBuffer::GetData(const char* key)
{
	static_assert((sizeof(T) % 16) == 0, "Constant Buffer size must be 16-byte aligned. Ensure that the size of the structure is a multiple of 16 bytes.");
	static_assert(!std::is_polymorphic_v<T>, "Constant Buffer cannot have virtual functions. The type must be a plain old data (POD) structure.");
	static_assert(std::is_trivially_destructible_v<T>, "Constant Buffer cannot have a destructor. The type must not be destructible.");

	std::shared_ptr<char[]> data = GetData(sizeof(T), key);

	// If this is the first reference, create a T object using placement new
	auto [iter, result] = initFlagSet.insert(make_key(sizeof(T), key));
	if (result)
		new(data.get()) T;

	std::shared_ptr<T> ptr = std::reinterpret_pointer_cast<T>(data);
	
	return ptr;
}

inline std::string D3DConstBuffer::make_key(size_t size_of, const char* key)
{
	std::string unique_key = std::to_string(size_of) + "_" + key;
	return unique_key;
}

template<>
inline void D3DConstBuffer::UpdateStaticCbuffer(const cb_Transform& data)
{
	ID3D11DeviceContext* pDeviceContext = d3dRenderer.GetDeviceContext();
	if constexpr (UPDATE_MODE_MAP)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Utility::CheckHRESULT(pDeviceContext->Map(cBufferTransform, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		// 상수 버퍼에 데이터를 쓴다.
		cb_Transform* dataPtr = (cb_Transform*)mappedResource.pData;
		*dataPtr = data;
		// 맵핑 해제
		pDeviceContext->Unmap(cBufferTransform, 0);
	}
	else
	{
		pDeviceContext->UpdateSubresource(cBufferTransform, 0, NULL, &data, 0, 0);
	}
}

template<>
inline void D3DConstBuffer::UpdateStaticCbuffer(const cb_Camera& data)
{
	ID3D11DeviceContext* pDeviceContext = d3dRenderer.GetDeviceContext();
	if constexpr (UPDATE_MODE_MAP)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Utility::CheckHRESULT(pDeviceContext->Map(cBufferCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		// 상수 버퍼에 데이터를 쓴다.
		cb_Camera* dataPtr = (cb_Camera*)mappedResource.pData;
		*dataPtr = data;
		// 맵핑 해제
		pDeviceContext->Unmap(cBufferCamera, 0);
	}
	else
	{
		pDeviceContext->UpdateSubresource(cBufferCamera, 0, NULL, &data, 0, 0);
	}
}

template<>
inline void D3DConstBuffer::UpdateStaticCbuffer(const cb_ShadowMap& data)
{
	ID3D11DeviceContext* pDeviceContext = d3dRenderer.GetDeviceContext();
	if constexpr (UPDATE_MODE_MAP)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Utility::CheckHRESULT(d3dRenderer.GetDeviceContext()->Map(cBufferShadowMap, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		// 상수 버퍼에 데이터를 쓴다.
		cb_ShadowMap* dataPtr = (cb_ShadowMap*)mappedResource.pData;
		*dataPtr = data;
		// 맵핑 해제
		d3dRenderer.GetDeviceContext()->Unmap(cBufferShadowMap, 0);
	}
	else
	{
		pDeviceContext->UpdateSubresource(cBufferShadowMap, 0, NULL, &data, 0, 0);
	}
}

template<typename T>
inline int D3DConstBuffer::CreateVSConstantBuffers(const char* key)
{
	static_assert((sizeof(T) % 16) == 0, "Constant Buffer size must be 16-byte aligned. Ensure that the size of the structure is a multiple of 16 bytes.");
	static_assert(!std::is_polymorphic_v<T>, "Constant Buffer cannot have virtual functions. The type must be a plain old data (POD) structure.");
	static_assert(std::is_trivially_destructible_v<T>, "Constant Buffer cannot have a destructor. The type must not be destructible.");

	int regindex = CreateVSConstantBuffers(sizeof(T), key);

	// If this is the first reference, create a T object using placement new
	auto [iter, result] = initFlagSet.insert(make_key(sizeof(T), key));
	if (result)
		new(vs_dataList[regindex - StaticCbufferCount].second.get())T;

	return regindex;
}

template<typename T>
inline int D3DConstBuffer::CreatePSConstantBuffers(const char* key)
{
	static_assert((sizeof(T) % 16) == 0, "Constant Buffer size must be 16-byte aligned. Ensure that the size of the structure is a multiple of 16 bytes.");
	static_assert(!std::is_polymorphic_v<T>, "Constant Buffer cannot have virtual functions. The type must be a plain old data (POD) structure.");
	static_assert(std::is_trivially_destructible_v<T>, "Constant Buffer cannot have a destructor. The type must not be destructible.");

	int regindex = CreatePSConstantBuffers(sizeof(T), key);

	// If this is the first reference, create a T object using placement new
	auto [iter, result] = initFlagSet.insert(make_key(sizeof(T), key));
	if (result)
		new(ps_dataList[regindex - StaticCbufferCount].second.get())T;

	return regindex;
}

inline void D3DConstBuffer::UpdateVSconstBuffer(int index)
{
	if (ID3D11DeviceContext* pDeviceContext = d3dRenderer.GetDeviceContext())
	{
		auto& [size, data] = vs_dataList[index];
		auto findIter = cbufferMap.find(size);
		if (findIter != cbufferMap.end())
		{
			if constexpr (UPDATE_MODE_MAP)
			{
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				Utility::CheckHRESULT(pDeviceContext->Map(findIter->second, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

				// 상수 버퍼에 데이터를 쓴다.
				memcpy(mappedResource.pData, data.get(), size);

				// 맵핑 해제
				pDeviceContext->Unmap(findIter->second, 0);
			}
			else
			{
				pDeviceContext->UpdateSubresource(cbufferMap[size], 0, NULL, data.get(), 0, 0);
			}
			//리소스 바인딩
			pDeviceContext->VSSetConstantBuffers(index + StaticCbufferCount, 1, &cbufferMap[size]);
		}
		else
		{
			__debugbreak(); //존재하지 않는 키.
			return;
		}
	}
}

inline void D3DConstBuffer::UpdatePSconstBuffer(int index)
{
	if (ID3D11DeviceContext* pDeviceContext = d3dRenderer.GetDeviceContext()) 
	{
		auto& [size, data] = ps_dataList[index];
		auto findIter = cbufferMap.find(size);
		if (findIter != cbufferMap.end())
		{
			if constexpr (UPDATE_MODE_MAP)
			{
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				Utility::CheckHRESULT(pDeviceContext->Map(findIter->second, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

				// 상수 버퍼에 데이터를 쓴다.
				memcpy(mappedResource.pData, data.get(), size);

				// 맵핑 해제
				pDeviceContext->Unmap(findIter->second, 0);
			}
			else
			{
				pDeviceContext->UpdateSubresource(cbufferMap[size], 0, NULL, data.get(), 0, 0);
			}
			//리소스 바인딩
			pDeviceContext->PSSetConstantBuffers(index + StaticCbufferCount, 1, &cbufferMap[size]);
		}
		else
		{
			__debugbreak(); //존재하지 않는 키.
			return;
		}
	}
}

