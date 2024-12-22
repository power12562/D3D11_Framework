#include "ImguiHelper.h"
#include "Math\Mathf.h"

#include <framework.h>
#include <D3DCore/D3DRenderer.h>

#include <unordered_map>
#include <string>
#include <memory>

static int g_id = 0;

void ImGui::ResetGlobalID()
{
	g_id = 0;
}

void ImGui::Button(const char* label, bool* v, const ImVec2& size)
{
	if (ImGui::Button(label, size))
	{
		*v = !(*v);
	}
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

void ImGui::DragQuaternionWorld(const char* label, const Quaternion* pQuaternion, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{	
	static std::unordered_map<int, Vector3> prevEuler;

	Quaternion* qu = const_cast<Quaternion*>(pQuaternion);
	Vector3 euler = qu->ToEuler() * Mathf::Rad2Deg;
	Vector3 prev = prevEuler[g_id];
	if (ImGui::DragFloat3(label, (float*)&euler, 1.f))
	{
		Quaternion deltaQuat = Quaternion::CreateFromYawPitchRoll(
			(euler.y - prev.y) * Mathf::Deg2Rad,
			(euler.x - prev.x) * Mathf::Deg2Rad,
			(euler.z - prev.z) * Mathf::Deg2Rad
		);
		if (deltaQuat.Length() > Mathf::Epsilon)
		{
			*qu = deltaQuat * (*qu);
			euler = qu->ToEuler() * Mathf::Rad2Deg;
		}
	}
    prevEuler[g_id] = euler;
	++g_id;
}

void ImGui::DragQuaternionLocal(const char* label, const Quaternion* pQuaternion, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{	
	static std::unordered_map<int, Vector3> prevEuler;

	Quaternion* qu = const_cast<Quaternion*>(pQuaternion);
	Vector3 euler = qu->ToEuler() * Mathf::Rad2Deg;
	Vector3 prev = prevEuler[g_id];
	if (ImGui::DragFloat3(label, (float*)&euler, 1.f))
	{
		Quaternion deltaQuat = Quaternion::CreateFromYawPitchRoll(
			(euler.y - prev.y) * Mathf::Deg2Rad,
			(euler.x - prev.x) * Mathf::Deg2Rad,
			(euler.z - prev.z) * Mathf::Deg2Rad
		);
		if (deltaQuat.Length() > Mathf::Epsilon)
		{
			*qu *= deltaQuat;
			euler = qu->ToEuler() * Mathf::Rad2Deg;
		}
	}
	prevEuler[g_id] = euler;
	++g_id;
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

void ImGui::EditTransformHierarchy(Transform* pTransform)
{
	auto ObjectEditUI = [](GameObject* object)
		{
			ImGui::Checkbox("Active", &object->Active);
			if (object->transform.Parent)
			{
				ImGui::DragVector3("Position", &object->transform.localPosition, 0.1f);
				ImGui::DragQuaternionLocal("Rotation", &object->transform.localRotation);
				ImGui::DragVector3("Scale", &object->transform.localScale, 0.1f);
			}
			else
			{
				ImGui::DragVector3("Position", &object->transform.position, 0.1f);
				ImGui::DragQuaternionWorld("Rotation", &object->transform.rotation);
				ImGui::DragVector3("Scale", &object->transform.scale, 0.1f);
			}
		};
	std::function<void(Transform* transform)> TransformBFS = [&](Transform* transform)
		{
			g_id++;
			ImGui::PushID(g_id);
			unsigned int childCount = transform->GetChildCount();
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
			if (Scene::GuizmoSetting.SelectObject == &transform->gameObject)
				flags |= ImGuiTreeNodeFlags_DefaultOpen;

			if (ImGui::TreeNodeEx(transform->gameObject.GetNameToString().c_str(), flags))
			{
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Scene::GuizmoSetting.SelectObject = &transform->gameObject;
				}			
				ObjectEditUI(&transform->gameObject);
				for (size_t i = 0; i < childCount; ++i)
				{
					TransformBFS(transform->GetChild(i));
				}
				ImGui::TreePop();
			}
			else if(ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				Scene::GuizmoSetting.SelectObject = &transform->gameObject;
			}
			ImGui::PopID();	
		};

	TransformBFS(pTransform);
}

void ImGui::EditHierarchyView()
{
	const ObjectList& objects = sceneManager.GetObjectList();
	for (auto& object : objects)
	{
		if(object->transform.Parent == nullptr)
			EditTransformHierarchy(&object->transform);
	}
	g_id++;
}

void ImGui::EditD3DRenderer()
{
	ImGui::PushID(g_id);
	ImGui::Text("D3DRenderer");
	ImGui::ColorEdit3("Clear Color", &d3dRenderer.backgroundColor);
	ImGui::Checkbox("Lock Camera Frustum", &d3dRenderer.DebugLockCameraFrustum);
	ImGui::Checkbox("Draw Camera Frustum", &d3dRenderer.DebugDrawCameraFrustum);
	ImGui::Checkbox("Draw Light Frustum", &d3dRenderer.DebugDrawLightFrustum);
	ImGui::Checkbox("Draw Object Culling Box", &d3dRenderer.DebugDrawObjectCullingBox);
	ImGui::Text("Setting");
	ImGui::Checkbox("VSync", &d3dRenderer.setting.UseVSync);
	if (ImGui::Button("Toggle Fullscreen"))
		d3dRenderer.ToggleFullscreenMode();

	ImGui::Text("");
	ImGui::PopID();
	g_id++;
}

void ImGui::EditTransform(GameObject* gameObject)
{
	ImGui::PushID(g_id);
	if (gameObject->transform.Parent)
	{
		ImGui::DragVector3("Position", &gameObject->transform.localPosition, 0.1f);
		ImGui::DragQuaternionLocal("Rotation", &gameObject->transform.localRotation);
		ImGui::DragVector3("Scale", &gameObject->transform.localScale, 0.1f);
	}
	else
	{
		ImGui::DragVector3("Position", &gameObject->transform.position, 0.1f);
		ImGui::DragQuaternionWorld("Rotation", &gameObject->transform.rotation);
		ImGui::DragVector3("Scale", &gameObject->transform.scale, 0.1f);
	}
	ImGui::PopID();
	g_id++;
}

void ImGui::EditCamera(const char* label, Camera* pCamera, CameraMoveHelper* pCameraMoveHelper)
{
	ImGui::Text(label);
	EditTransform(&pCamera->gameObject);
	ImGui::PushID(g_id);
	ImGui::Checkbox("Draw Frustum", &d3dRenderer.DebugDrawCameraFrustum);
	ImGui::Checkbox("Lock Frustum", &d3dRenderer.DebugLockCameraFrustum);
	ImGui::SliderFloat("FOV", &pCamera->FOV, 10, 120);
	ImGui::SliderFloat("Near", &pCamera->Near, 0.05f, 10.f);
	ImGui::SliderFloat("Far", &pCamera->Far, 15.f, 10000.f);
	if (pCameraMoveHelper)
	{
		ImGui::DragFloat("Move Speed", &pCameraMoveHelper->moveSpeed, 1.f, 1.f, 1000.f);
		ImGui::DragFloat("Rotation Speed", &pCameraMoveHelper->rotSpeed, 0.1f, 0.1f, 1.f);
	}
	ImGui::Text("");
	ImGui::PopID();
	g_id++;
}

void ImGui::EditMaterial(const char* label, cb_PBRMaterial* Material)
{
	ImGui::Text(label);
	ImGui::PushID(g_id);

	ImGui::ColorEdit4("Albedo", &Material->Albedo);
	ImGui::SliderFloat("Metalness", &Material->Metalness, 0.f, 1.0f);
	ImGui::SliderFloat("Roughness", &Material->Roughness, 0.f, 1.0f);

	ImGui::Checkbox("Metalness Map", &Material->UseMetalnessMap);
	ImGui::Checkbox("Roughness Map", &Material->UseRoughnessMap);
	ImGui::Checkbox("RMAC Map", &Material->UseRMACMap);

	ImGui::Text("");
	ImGui::PopID();
	g_id++;
}

void ImGui::EditLight(cb_PBRDirectionalLight* Light)
{
	constexpr char label[cb_PBRDirectionalLight::MAX_LIGHT_COUNT][20] = {
		{"Directional Light 1"},
		{"Directional Light 2"},
		{"Directional Light 3"},
		{"Directional Light 4"} };
	if (Button("Add Directional Light"))
		Light->PushLight();
	if (Button("Sub Directional Light"))
		Light->PopLight();

	for (int i = 0; i < Light->LightsCount; i++)
	{
		ImGui::Text(label[i]);
		ImGui::PushID(g_id);
		ImGui::ColorEdit4("Color", &Light->Lights[i].LightColor);
		ImGui::DragVector3("Dir", &Light->Lights[i].LightDir, 0.01f, -1.f, 1.f);
		ImGui::DragFloat("Intensity", &Light->Lights[i].LightIntensity, 1.f, 0.0000001f, 100.f);
		ImGui::Text("");
		ImGui::PopID();
		g_id++;
	}
}