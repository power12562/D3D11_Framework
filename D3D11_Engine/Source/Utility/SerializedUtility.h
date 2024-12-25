#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <type_traits>
#include <DirectXCollision.h>
#include <directxtk/SimpleMath.h>

namespace Binary
{
	namespace Write
	{
		template<typename T> void data(std::ofstream& ofs, const T data)
		{
			static_assert(std::is_integral_v<T>
				|| std::is_floating_point_v<T>
				|| std::is_same_v<T, bool>,
				"Type must be a basic type (int, char, bool, size_t, unsigned long, etc.)");
			ofs.write(reinterpret_cast<const char*>(&data), sizeof(T));
		}

		void string(std::ofstream& ofs, const std::string& data);
		void wstring(std::ofstream& ofs, const std::wstring& data);
		void floatArray(std::ofstream& ofs, const float* data, size_t size);

		void Vector3(std::ofstream& ofs, const DirectX::SimpleMath::Vector3& data);
		void Quaternion(std::ofstream& ofs, const DirectX::SimpleMath::Quaternion& data);
		void BoundingBox(std::ofstream& ofs, const DirectX::BoundingBox& data);
	}
	namespace Read 
	{
		template<typename T> T data(std::ifstream& ifs)
		{
			static_assert(std::is_integral_v<T>
				|| std::is_floating_point_v<T>
				|| std::is_same_v<T, bool>,
				"Type must be a basic type (int, char, bool, size_t, unsigned long, etc.)");
			T data{};
			ifs.read(reinterpret_cast<char*>(&data), sizeof(T));
			return data;
		}

		std::string string(std::ifstream& ifs);
		std::wstring wstring(std::ifstream& ifs);
		void floatArray(std::ifstream& ifs, float* out, size_t size);

		DirectX::SimpleMath::Vector3 Vector3(std::ifstream& ifs);
		DirectX::SimpleMath::Quaternion Quaternion(std::ifstream& ifs);
		DirectX::BoundingBox BoundingBox(std::ifstream& ifs);

	}
}