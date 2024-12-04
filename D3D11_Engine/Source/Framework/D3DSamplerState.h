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

	ID3D11SamplerState* operator[](int index) { return samplerList[index]; }

public:
	D3DSamplerState(const D3DSamplerState& rhs);
	D3DSamplerState& operator=(const D3DSamplerState& rhs);

private:
	std::vector<ID3D11SamplerState*> samplerList;
};