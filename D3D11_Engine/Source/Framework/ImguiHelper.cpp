#include "ImguiHelper.h"
#include "Math\Mathf.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace ImGui
{
	auto arrDeleter = [](float* b)->void
	{
		delete[] b;
	};
	std::unordered_map<std::string, std::unique_ptr<float, decltype(arrDeleter)>> tempFloatMap;
	static std::unique_ptr<float, decltype(arrDeleter)>& GetTemp(const char* key, int size, float* init)
	{
		auto findIter = tempFloatMap.find(key);
		if (findIter != tempFloatMap.end())
		{
			return tempFloatMap[key];
		}
		else
		{
			tempFloatMap[key].reset(new float[size]);
			for (int i = 0; i < size; i++)
			{
				tempFloatMap[key].get()[i] = init[i];
			}
			return tempFloatMap[key];
		}	
	}
};

void ImGui::DragVector2(const char* label, const Vector2* pVector, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{	
	auto& temp = GetTemp(label, 2, (float*)pVector);
	ImGui::DragFloat3(label, temp.get(), v_speed, v_min, v_max, format, flags);
	Vector2* vec = const_cast<Vector2*>(pVector);
	*vec = Vector2(*temp);
}

void ImGui::DragVector3(const char* label, const Vector3* pVector, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	auto& temp = GetTemp(label, 3, (float*)pVector);
	ImGui::DragFloat3(label, temp.get(), v_speed, v_min, v_max, format, flags);
	Vector3* vec = const_cast<Vector3*>(pVector);
	*vec = Vector3(*temp);
}

void ImGui::DragVector4(const char* label, const Vector4* pVector, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	auto& temp = GetTemp(label, 4, (float*)pVector);
	ImGui::DragFloat3(label, temp.get(), v_speed, v_min, v_max, format, flags);
	Vector4* vec = const_cast<Vector4*>(pVector);
	*vec = Vector4(*temp);
}

void ImGui::DragQuaternion(const char* label, const Quaternion* pQuaternion, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	auto& temp = GetTemp(label, 3, (float*)pQuaternion);
	ImGui::DragFloat3(label, temp.get(), v_speed, v_min, v_max, format, flags);
	Quaternion* qua = const_cast<Quaternion*>(pQuaternion);
	*qua = Quaternion::CreateFromYawPitchRoll(Vector3(*temp) * Mathf::Deg2Rad);
}

void ImGui::ColorEdit3(const char* label, const Vector3* pColor, ImGuiColorEditFlags flags)
{
	auto& temp = GetTemp(label, 3, (float*)pColor);
	ImGui::ColorEdit3(label, temp.get(), flags);
	Vector3* col = const_cast<Vector3*>(pColor);
	*col = Vector3(*temp);
}

void ImGui::ColorEdit3(const char* label, const Color* pColor, ImGuiColorEditFlags flags)
{
	auto& temp = GetTemp(label, 4, (float*)pColor);
	ImGui::ColorEdit3(label, temp.get(), flags);
	Color* col = const_cast<Color*>(pColor);
	*col = Color(*temp);
}

void ImGui::ColorEdit3(const char* label, const Vector4* pColor, ImGuiColorEditFlags flags)
{
	auto& temp = GetTemp(label, 4, (float*)pColor);
	ImGui::ColorEdit3(label, temp.get(), flags);
	Vector4* col = const_cast<Vector4*>(pColor);
	*col = Vector4(*temp);
}

void ImGui::ColorEdit4(const char* label, const Color* pColor, ImGuiColorEditFlags flags)
{
	auto& temp = GetTemp(label, 4, (float*)pColor);
	ImGui::ColorEdit4(label, temp.get(), flags);
	Color* col = const_cast<Color*>(pColor);
	*col = Color(*temp);
}

void ImGui::ColorEdit4(const char* label, const Vector4* pColor, ImGuiColorEditFlags flags)
{
	auto& temp = GetTemp(label, 4, (float*)pColor);
	ImGui::ColorEdit4(label, temp.get(), flags);
	Vector4* col = const_cast<Vector4*>(pColor);
	*col = Vector4(*temp);
}
