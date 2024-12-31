#pragma once
#include <Core/Transform.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

class GameObject;
class CameraMoveHelper;
class Camera;
class D3DTexture2D;
struct cb_PBRMaterial;
struct PBRDirectionalLight;
struct cb_PBRDirectionalLight;
struct ID3D11ShaderResourceView;
namespace ImGui
{
	void ResetGlobalID();

	void Button(const char* label, bool* v, const ImVec2& size = ImVec2(0, 0));

	bool DragVector2(const char* label, const Vector2* pVector, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragVector3(const char* label, const Vector3* pVector, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragVector4(const char* label, const Vector4* pVector, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

	bool DragQuaternionWorld(const char* label, const Quaternion* pQuaternion, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool DragQuaternionLocal(const char* label, const Quaternion* pQuaternion, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	
	bool ColorEdit3(const char* label, const Vector3* pColor, ImGuiColorEditFlags flags = 0);
	bool ColorEdit3(const char* label, const Color* pColor, ImGuiColorEditFlags flags = 0);
	bool ColorEdit3(const char* label, const Vector4* pColor, ImGuiColorEditFlags flags = 0);

	bool ColorEdit4(const char* label, const Color* pColor, ImGuiColorEditFlags flags = 0);
	bool ColorEdit4(const char* label, const Vector4* pColor, ImGuiColorEditFlags flags = 0);

	void EditTransformHierarchy(Transform* pTransform);
	void EditHierarchyView();

	void EditD3DRenderer();

	void EditTransform(GameObject* gameObject);

	void EditCamera(const char* label, Camera* pCamera, CameraMoveHelper* pCameraMoveHelper = nullptr);

	void EditMaterial(const char* label, cb_PBRMaterial* Material);

	void EditLight(cb_PBRDirectionalLight* Light);

	/*���� �˾� Ȱ��ȭ.*/
	bool ShowCompressPopup(const wchar_t* path, D3DTexture2D* texture2D, int texType);
	/*�������� �ؽ��� �����ϸ� ���� ������ �ٽ� �ε��ϵ��� ����*/
	bool ReloadTextureCompressEnd(const wchar_t* path, D3DTexture2D* texture2D, int texType);

	/*���� ������Ʈ ���� �˾� Ȱ��ȭ*/
	bool ShowOpenGameObjectPopup();

	/*���� ������Ʈ ���� �˾� Ȱ��ȭ*/
	bool ShowSaveAsGameObjectPopup(GameObject* object);
};