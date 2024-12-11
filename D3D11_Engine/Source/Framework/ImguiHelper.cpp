#include "ImguiHelper.h"
#include "Math\Mathf.h"

#include <GameObject/Base/GameObject.h>
#include <Component/Camera/Camera.h>
#include <Component/Camera/CameraMoveHelper.h>

#include <unordered_map>
#include <string>
#include <memory>

static int g_id = 0;
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

void ImGui::ClearTempMap()
{
	tempFloatMap.clear();
}

void ImGui::ResetGlobalID()
{
	g_id = 0;
}

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
	auto& temp = GetTemp(label, 3);
	Vector3 previousRotation = pQuaternion->ToEuler() * Mathf::Rad2Deg;

	temp.get()[0] = previousRotation.x;
	temp.get()[1] = previousRotation.y;
	temp.get()[2] = previousRotation.z;

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

void ImGui::EditTransform(GameObject* gameObject)
{
	ImGui::PushID(g_id);
	ImGui::DragVector3("Position", &gameObject->transform.position, 0.1f);
	ImGui::DragQuaternion("Rotation", &gameObject->transform.rotation);
	ImGui::DragVector3("Scale", &gameObject->transform.scale, 0.1f);
	ImGui::PopID();
	g_id++;
}

void ImGui::EditCamera(const char* label, Camera* pCamera, CameraMoveHelper* pCameraMoveHelper)
{
	ImGui::Text(label);
	EditTransform(&pCamera->gameObject);
	ImGui::PushID(g_id);
	ImGui::SliderFloat("FOV", &pCamera->FOV, 10, 120);
	ImGui::SliderFloat("Near", &pCamera->Near, 0.05f, 10.f);
	ImGui::SliderFloat("Far", &pCamera->Far, 15.f, 10000.f);
	if (pCameraMoveHelper)
	{
		ImGui::DragFloat("Move Speed", &pCameraMoveHelper->moveSpeed, 1.f, 1.f, 1000.f);
		ImGui::DragFloat("Rotation Speed", &pCameraMoveHelper->rotSpeed, 1.f, 1.f, 30.f);
	}
	ImGui::Text("");
	ImGui::PopID();
	g_id++;
}
