#include "D3DSamplerState.h"
#include <Utility/MemoryUtility.h>
#include <Framework/D3DRenderer.h>

using namespace Utility;

D3DSamplerState::~D3DSamplerState()
{
	for (auto& item : samplerList)
	{
		SafeRelease(item);
	}
}

void D3DSamplerState::resize(size_t newsize)
{
	if (newsize < D3DSamplerState::MAX_SIZE)
	{
		samplerList.resize(newsize);
	}
	else
	{
		__debugbreak();
		//최대 크기 초과
	}
}

void D3DSamplerState::SetSamplerState(int index, D3D11_SAMPLER_DESC& desc)
{
	samplerList[index]->Release();
	CheckHRESULT(d3dRenderer.GetDevice()->CreateSamplerState(&desc, &samplerList[index]));
}

void D3DSamplerState::ResetSamplerState(int index)
{
	samplerList[index]->Release();
}
