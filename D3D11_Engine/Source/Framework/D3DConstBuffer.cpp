#include "D3DConstBuffer.h"
#include <Framework/D3DRenderer.h>

void D3DConstBuffer::InitStaticCbuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // ��� ���۴� dynamic���� �����ϰ� ���� ����.
	bufferDesc.ByteWidth = sizeof(cb_Transform);  // ��� ���� ũ��
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Utility::CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferTransform));

	bufferDesc.ByteWidth = sizeof(cb_Camera); 
	Utility::CheackHRESULT(d3dRenderer.GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cBufferCamera));
}

void D3DConstBuffer::UninitStaticCbuffer()
{
	using namespace Utility;
	SafeRelease(cBufferTransform);
	SafeRelease(cBufferCamera);
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

	for (int i = 0; i < vs_cbufferList.size(); i++)
		pDeviceContext->VSSetConstantBuffers(i + StaticCbufferCount, 1, &cBufferMap[vs_cbufferList[i]]);

	for (int i = 0; i < ps_cbufferList.size(); i++)
		pDeviceContext->PSSetConstantBuffers(i + StaticCbufferCount, 1, &cBufferMap[ps_cbufferList[i]]);
}
