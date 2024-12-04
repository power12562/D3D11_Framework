#pragma once
#include <Utility/D3D11Utility.h>
#include <string>

class D3DTexture2D
{
	static const size_t MAX_SIZE = 128;
public:
	D3DTexture2D() = default;
	~D3DTexture2D();	

	void resize(size_t newsize);
	constexpr size_t size() { return SRVList.size(); }

	void SetTexture2D(int index, const wchar_t* path);
	void SetOneTexture(int index);
	void SetZeroTexture(int index);
	void SetDefaultNormalTexture(int index);
	void ResetTexture2D(int index);

	ID3D11ShaderResourceView* operator[](int index) { return SRVList[index]; }
public:
	D3DTexture2D(const D3DTexture2D& rhs);
	D3DTexture2D& operator=(const D3DTexture2D& rhs);

private:
	std::vector<ID3D11ShaderResourceView*> SRVList{};
	std::vector<std::wstring> pathList{};
};

