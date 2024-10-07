#include "ImguiHelper.h"
#include "Math\Mathf.h"

void ImGui::DragVector3(const char* label, const Vector3* pVector, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	static float temp[3]{ pVector->x, pVector->y, pVector->z };
	ImGui::DragFloat3(label, temp, v_speed, v_min, v_max, format, flags);
	Vector3* vec = const_cast<Vector3*>(pVector);
	*vec = Vector3(temp);
}

void ImGui::DragQuaternion(const char* label, const Quaternion* pQuaternion, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	Vector3 euler = pQuaternion->ToEuler() * Mathf::Rad2Deg;
	static float temp[3]{ euler.x, euler.y, euler.z };

	ImGui::DragFloat3(label, temp, v_speed, v_min, v_max, format, flags);
	Quaternion* qua = const_cast<Quaternion*>(pQuaternion);
	*qua = Quaternion::CreateFromYawPitchRoll(Vector3(temp) * Mathf::Deg2Rad);
}
