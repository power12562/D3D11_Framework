#include "D3DConstBuffer.h"
#include <D3DCore/D3DRenderer.h>

void D3DConstBuffer::CreateStaticCbuffer()
{
	D3D11_BUFFER_DESC bufferDesc{};
	if constexpr (UPDATE_MODE_MAP)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // 상수 버퍼는 dynamic으로 생성하고 쓰기 전용.
		bufferDesc.ByteWidth = sizeof(cb_Transform); 
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		bufferDesc.Usage = D3D11_USAGE_DEFAULT; 
		bufferDesc.ByteWidth = sizeof(cb_Transform); 
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	}
	Utility::CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferTransform));

	bufferDesc.ByteWidth = sizeof(cb_Camera); 
	Utility::CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferCamera));

	bufferDesc.ByteWidth = sizeof(cb_ShadowMap);
	Utility::CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferShadowMap));
}

void D3DConstBuffer::ReleaseStaticCbuffer()
{
	using namespace Utility;
	SafeRelease(cBufferTransform);
	SafeRelease(cBufferCamera);
	SafeRelease(cBufferShadowMap);

	for (auto& buffer : cbufferMap)
	{
		SafeRelease(buffer.second);
	}
}

void D3DConstBuffer::SetStaticCbuffer()
{
	auto pDeviceContext = d3dRenderer.GetDeviceContext();
	pDeviceContext->VSSetConstantBuffers(0, 1, &cBufferTransform);
	pDeviceContext->VSSetConstantBuffers(1, 1, &cBufferCamera);
	pDeviceContext->VSSetConstantBuffers(2, 1, &cBufferShadowMap);
	pDeviceContext->PSSetConstantBuffers(0, 1, &cBufferTransform);
	pDeviceContext->PSSetConstantBuffers(1, 1, &cBufferCamera);
	pDeviceContext->PSSetConstantBuffers(2, 1, &cBufferShadowMap);
}

std::shared_ptr<char[]> D3DConstBuffer::GetData(size_t size_of, const char* key)
{
	std::string unique_key = make_key(size_of, key);
	auto find = dataMap.find(unique_key);
	if (find != dataMap.end())
	{
		auto& [key, wptr] = *find;
		if (!wptr.expired())
		{
			return wptr.lock();
		}		
	}

	//리소스 생성
	std::shared_ptr<char[]> newData(new char[size_of]);
	dataMap[unique_key] = newData;
	return newData;
}

D3DConstBuffer::D3DConstBuffer()
{
   
}

D3DConstBuffer::~D3DConstBuffer()
{
	for (auto& [key, data] : vs_dataList)
	{
		ULONG ref = cbufferMap[key]->Release();
		if (ref == 0)
		{
			cbufferMap.erase(key);
		}
	}
	for (auto& [key, data] : ps_dataList)
	{
		ULONG ref = cbufferMap[key]->Release();
		if (ref == 0)
		{
			cbufferMap.erase(key);
		}
	}
}

void D3DConstBuffer::SetConstBuffer()
{
	for (int i = 0; i < vs_keyList.size(); i++)
	{
		UpdateVSconstBuffer(i);
	}

	for (int i = 0; i < ps_keyList.size(); i++)
	{
		UpdatePSconstBuffer(i);
	}		
}

int D3DConstBuffer::CreateVSConstantBuffers(size_t size_of, const char* key)
{
	if (GetVSCbufferCount() == D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
	{
		__debugbreak(); //상수 버퍼 최대 개수 도달.
		return -1;
	}
	std::string unique_key = make_key(size_of, key);
	int regIndex = GetVSCbufferCount();
	if (cbufferMap.find(size_of) == cbufferMap.end()) //키값 못찾을때 리소스 생성
	{
		D3D11_BUFFER_DESC bufferDesc{};

		if constexpr (UPDATE_MODE_MAP)
		{
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // 상수 버퍼는 dynamic으로 생성하고 쓰기 전용.
			bufferDesc.ByteWidth = (UINT)size_of;  // 상수 버퍼 크기
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else  //test
		{
			bufferDesc.Usage = D3D11_USAGE_DEFAULT; // 상수 버퍼는 dynamic으로 생성하고 쓰기 전용.
			bufferDesc.ByteWidth = (UINT)size_of;  // 상수 버퍼 크기
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
		}

		ID3D11Buffer* cBufferTemp{};
		Utility::CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferTemp));
		cbufferMap[size_of] = cBufferTemp;
	}
	else
	{
		for (int i = 0; i < vs_keyList.size(); i++)
		{
			if (vs_keyList[i] == unique_key)
				return i;
		}
		cbufferMap[size_of]->AddRef();
	}
	std::shared_ptr<char[]> data = GetData(size_of, key);
	vs_dataList.emplace_back(size_of, data);
	vs_keyList.emplace_back(unique_key);

	return regIndex;
}

int D3DConstBuffer::CreatePSConstantBuffers(size_t size_of, const char* key)
{
	if (GetPSCbufferCount() == D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
	{
		__debugbreak(); //상수 버퍼 최대 개수 도달.
		return -1;
	}
	std::string unique_key = make_key(size_of, key);
	int regIndex = GetPSCbufferCount();
	if (cbufferMap.find(size_of) == cbufferMap.end()) //키값 못찾을때 리소스 생성
	{
		D3D11_BUFFER_DESC bufferDesc{};
		if constexpr (UPDATE_MODE_MAP)
		{
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // 상수 버퍼는 dynamic으로 생성하고 쓰기 전용.
			bufferDesc.ByteWidth = (UINT)size_of;  // 상수 버퍼 크기
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else //test
		{
			bufferDesc.Usage = D3D11_USAGE_DEFAULT; // 상수 버퍼는 dynamic으로 생성하고 쓰기 전용.
			bufferDesc.ByteWidth = (UINT)size_of;  // 상수 버퍼 크기
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
		}

		ID3D11Buffer* cBufferTemp{};
		Utility::CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferTemp));
		cbufferMap[size_of] = cBufferTemp;
	}
	else
	{
		for (int i = 0; i < ps_keyList.size(); i++)
		{
			if (ps_keyList[i] == unique_key)
				return i;
		}
		cbufferMap[size_of]->AddRef();
	}
	std::shared_ptr<char[]> data = GetData(size_of, key);
	ps_dataList.emplace_back(size_of, data);
	ps_keyList.emplace_back(unique_key);
	
	return regIndex;
}
