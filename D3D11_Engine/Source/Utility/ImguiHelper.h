#pragma once
#include <Core/Transform.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

class GameObject;
class CameraMoveHelper;
class Camera;
struct cb_PBRMaterial;
struct PBRDirectionalLight;
struct cb_PBRDirectionalLight;
struct ID3D11ShaderResourceView;
namespace ImGui
{
	void ResetGlobalID();

	void Button(const char* label, bool* v, const ImVec2& size = ImVec2(0, 0));

	void DragVector2(const char* label, const Vector2* pVector, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	void DragVector3(const char* label, const Vector3* pVector, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	void DragVector4(const char* label, const Vector4* pVector, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	void DragQuaternionWorld(const char* label, const Quaternion* pQuaternion, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	void DragQuaternionLocal(const char* label, const Quaternion* pQuaternion, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	
	void ColorEdit3(const char* label, const Vector3* pColor, ImGuiColorEditFlags flags = 0);
	void ColorEdit3(const char* label, const Color* pColor, ImGuiColorEditFlags flags = 0);
	void ColorEdit3(const char* label, const Vector4* pColor, ImGuiColorEditFlags flags = 0);

	void ColorEdit4(const char* label, const Color* pColor, ImGuiColorEditFlags flags = 0);
	void ColorEdit4(const char* label, const Vector4* pColor, ImGuiColorEditFlags flags = 0);

	void EditTransformHierarchy(Transform* pTransform);
	void EditHierarchyView();

	void EditD3DRenderer();

	void EditTransform(GameObject* gameObject);

	void EditCamera(const char* label, Camera* pCamera, CameraMoveHelper* pCameraMoveHelper = nullptr);

	void EditMaterial(const char* label, cb_PBRMaterial* Material);

	void EditLight(cb_PBRDirectionalLight* Light);

	void ShowCompressPopup(const wchar_t* path, ID3D11ShaderResourceView** ppOut_ResourceView);
};