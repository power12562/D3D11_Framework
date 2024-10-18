#include "D3DConstBuffer.h"
#include <Framework/D3DRenderer.h>

void D3DConstBuffer::CreateStaticCbuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // 상수 버퍼는 dynamic으로 생성하고 쓰기 전용.
	bufferDesc.ByteWidth = sizeof(cb_Transform);  // 상수 버퍼 크기
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Utility::CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferTransform));

	bufferDesc.ByteWidth = sizeof(cb_Camera); 
	Utility::CheckHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferCamera));
}

void D3DConstBuffer::ReleaseStaticCbuffer()
{
	using namespace Utility;
	SafeRelease(cBufferTransform);
	SafeRelease(cBufferCamera);

	for (auto& buffer : cBufferMap)
	{
		SafeRelease(buffer.second);
	}
}

D3DConstBuffer::D3DConstBuffer()
{
   
}

D3DConstBuffer::~D3DConstBuffer()
{

}

void D3DConstBuffer::SetConstBuffer()
{
	auto pDeviceContext = d3dRenderer.GetDeviceContext();

	pDeviceContext->VSSetConstantBuffers(0, 1, &cBufferTransform);
	pDeviceContext->VSSetConstantBuffers(1, 1, &cBufferCamera);
	for (int i = 0; i < vs_cbufferList.size(); i++)
		pDeviceContext->VSSetConstantBuffers(i + StaticCbufferCount, 1, &cBufferMap[vs_cbufferList[i]]);

	pDeviceContext->PSSetConstantBuffers(0, 1, &cBufferTransform);
	pDeviceContext->PSSetConstantBuffers(1, 1, &cBufferCamera);
	for (int i = 0; i < ps_cbufferList.size(); i++)
		pDeviceContext->PSSetConstantBuffers(i + StaticCbufferCount, 1, &cBufferMap[ps_cbufferList[i]]);
}

void D3DConstBuffer::UpdateEvent()
{
	for (auto& even : cb_updateEvents)
	{
		even();
	}
}
