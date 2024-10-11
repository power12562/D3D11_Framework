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
	static std::unique_ptr<float, decltype(arrDeleter)>& GetTemp(const char* key, int size)
	{
		auto findIter = tempFloatMap.find(key);
		if (findIter != tempFloatMap.end())
		{
			return tempFloatMap[key];
		}
		else
		{
			tempFloatMap[key].reset(new float[size]{});
			return tempFloatMap[key];
		}
	}
};

void ImGui::DragVector2(const char* label, const Vector2* pVector, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{	
	ImGui::DragFloat3(label, (float*)pVector, v_speed, v_min, v_max, format, flags);
}

void ImGui::DragVector3(const char* label, const Vector3* pVector, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	ImGui::DragFloat3(label, (float*)pVector, v_speed, v_min, v_max, format, flags);
}

void ImGui::DragVector4(const char* label, const Vector4* pVector, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	ImGui::DragFloat3(label,  (float*)pVector, v_speed, v_min, v_max, format, flags);
}

void ImGui::DragQuaternion(const char* label, const Quaternion* pQuaternion, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	Vector3 euler = pQuaternion->ToEuler() * Mathf::Rad2Deg;
	auto& temp = GetTemp(label, 3);
	temp.get()[0] = euler.x;
	temp.get()[1] = euler.y;
	temp.get()[2] = euler.z;
	ImGui::DragFloat3(label, temp.get(), v_speed, v_min, v_max, format, flags);
	Quaternion* qu = const_cast<Quaternion*>(pQuaternion);
	*qu = Quaternion::CreateFromYawPitchRoll(Vector3(temp.get()) * Mathf::Deg2Rad);
}

void ImGui::ColorEdit3(const char* label, const Vector3* pColor, ImGuiColorEditFlags flags)
{
	ImGui::ColorEdit3(label, (float*)pColor, flags);
}

void ImGui::ColorEdit3(const char* label, const Color* pColor, ImGuiColorEditFlags flags)
{
	ImGui::ColorEdit3(label, (float*)pColor, flags);
}

void ImGui::ColorEdit3(const char* label, const Vector4* pColor, ImGuiColorEditFlags flags)
{
	ImGui::ColorEdit3(label, (float*)pColor, flags);
}

void ImGui::ColorEdit4(const char* label, const Color* pColor, ImGuiColorEditFlags flags)
{
	ImGui::ColorEdit4(label, (float*)pColor, flags);
}

void ImGui::ColorEdit4(const char* label, const Vector4* pColor, ImGuiColorEditFlags flags)
{
	ImGui::ColorEdit4(label, (float*)pColor, flags);
}
