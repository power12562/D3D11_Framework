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
	ResetSamplerState(index);
	CheckHRESULT(d3dRenderer.GetDevice()->CreateSamplerState(&desc, &samplerList[index]));
}

void D3DSamplerState::ResetSamplerState(int index)
{
	if (samplerList[index])
	{
		samplerList[index]->Release();
		samplerList[index] = nullptr;
	}
}

D3DSamplerState::D3DSamplerState(const D3DSamplerState& rhs)
{
	if (this == &rhs)
		return;

	this->samplerList = rhs.samplerList;
	for (auto& sampler : samplerList)
	{
		sampler->AddRef();
	}
}

D3DSamplerState& D3DSamplerState::operator=(const D3DSamplerState& rhs)
{
	if (this == &rhs)
		return *this;

	for (auto& sampler : samplerList)
	{
		sampler->Release();
	}
	this->samplerList = rhs.samplerList;
	for (auto& sampler : samplerList)
	{
		sampler->AddRef();
	}

	return *this;
}
