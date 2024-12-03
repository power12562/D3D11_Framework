#pragma once
#include <Utility/D3D11Utility.h>

class D3DSamplerState
{
	static const size_t MAX_SIZE = 16;
public:
	D3DSamplerState() = default;
	~D3DSamplerState();

	void resize(size_t newsize);
	constexpr size_t size() { return samplerList.size(); }

	void SetSamplerState(int index, D3D11_SAMPLER_DESC& desc);
	void ResetSamplerState(int index);
private:
	std::vector<ID3D11SamplerState*> samplerList;
};