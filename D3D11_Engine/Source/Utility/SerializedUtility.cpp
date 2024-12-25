#include "SerializedUtility.h"

void Binary::Write::string(std::ofstream& ofs, const std::string& data)
{
	size_t len = data.length();
	ofs.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
	ofs.write(data.c_str(), len);
	return;
}

std::string Binary::Read::string(std::ifstream& ifs)
{
	std::string data;
	size_t len = 0;
	ifs.read(reinterpret_cast<char*>(&len), sizeof(size_t));
	data.resize(len);
	ifs.read(reinterpret_cast<char*>(data.data()), len);
	return data;
}

void Binary::Write::wstring(std::ofstream& ofs, const std::wstring& data)
{
	size_t len = data.length();
	ofs.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
	ofs.write(reinterpret_cast<const char*>(data.c_str()), len * sizeof(wchar_t));
	return;
}

std::wstring Binary::Read::wstring(std::ifstream& ifs)
{
	std::wstring data;
	size_t len = 0;
	ifs.read(reinterpret_cast<char*>(&len), sizeof(size_t));
	data.resize(len);
	ifs.read(reinterpret_cast<char*>(data.data()), len * sizeof(wchar_t));
	return data;
}

void Binary::Write::floatArray(std::ofstream& ofs, const float* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		Write::data(ofs, data[i]);
	}
}

void Binary::Read::floatArray(std::ifstream& ifs, float* out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[i] = Read::data<float>(ifs);		
	}
}

void Binary::Write::BoundingBox(std::ofstream& ofs, const DirectX::BoundingBox& data)
{
	Write::floatArray(ofs, reinterpret_cast<const float*>(&data.Center), 3);
	Write::floatArray(ofs, reinterpret_cast<const float*>(&data.Extents), 3);
}

DirectX::BoundingBox Binary::Read::BoundingBox(std::ifstream& ifs)
{
	float Center[3]{};
	Read::floatArray(ifs, Center, 3);
	float Extents[3]{};
	Read::floatArray(ifs, Extents, 3);
	DirectX::BoundingBox data{DirectX::XMFLOAT3(Center), DirectX::XMFLOAT3(Extents)};
	return data;
}

void Binary::Write::Vector3(std::ofstream& ofs, const DirectX::SimpleMath::Vector3& data)
{
	Write::floatArray(ofs, reinterpret_cast<const float*>(&data), 3);
}

DirectX::SimpleMath::Vector3 Binary::Read::Vector3(std::ifstream& ifs)
{
	float data[3]{};
	Read::floatArray(ifs, data, 3);
	return DirectX::SimpleMath::Vector3(data);
}

void Binary::Write::Quaternion(std::ofstream& ofs, const DirectX::SimpleMath::Quaternion& data)
{
	Write::floatArray(ofs, reinterpret_cast<const float*>(&data), 4);
}

DirectX::SimpleMath::Quaternion Binary::Read::Quaternion(std::ifstream& ifs)
{
	float data[4]{};
	Read::floatArray(ifs, data, 4);
	return DirectX::SimpleMath::Quaternion(data);
}
