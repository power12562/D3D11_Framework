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

bool ImGui::DragVector2(const char* label, const Vector2* pVector, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{	
	return ImGui::DragFloat3(label, (float*)pVector, v_speed, v_min, v_max, format, flags);
}

bool ImGui::DragVector3(const char* label, const Vector3* pVector, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragFloat3(label, (float*)pVector, v_speed, v_min, v_max, format, flags);
}

bool ImGui::DragVector4(const char* label, const Vector4* pVector, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::DragFloat3(label,  (float*)pVector, v_speed, v_min, v_max, format, flags);
}

bool ImGui::DragQuaternionWorld(const char* label, const Quaternion* pQuaternion, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{	
	static std::unordered_map<Quaternion*, Vector3> prevEuler;
	bool isEdit = false;
	Quaternion* qu = const_cast<Quaternion*>(pQuaternion);
	Vector3 euler = qu->ToEuler() * Mathf::Rad2Deg;
	Vector3 prev = prevEuler[qu];
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
			isEdit = true;
		}
	}
    prevEuler[qu] = euler;
	return isEdit;
}

bool ImGui::DragQuaternionLocal(const char* label, const Quaternion* pQuaternion, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{	
	static std::unordered_map<Quaternion*, Vector3> prevEuler;
	bool isEdit = false;
	Quaternion* qu = const_cast<Quaternion*>(pQuaternion);
	Vector3 euler = qu->ToEuler() * Mathf::Rad2Deg;
	Vector3 prev = prevEuler[qu];
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
			isEdit = true;
		}
	}
	prevEuler[qu] = euler;
	return isEdit;
}

bool ImGui::ColorEdit3(const char* label, const Vector3* pColor, ImGuiColorEditFlags flags)
{
	return ImGui::ColorEdit3(label, (float*)pColor, flags);
}

bool ImGui::ColorEdit3(const char* label, const Color* pColor, ImGuiColorEditFlags flags)
{
	return ImGui::ColorEdit3(label, (float*)pColor, flags);
}

bool ImGui::ColorEdit3(const char* label, const Vector4* pColor, ImGuiColorEditFlags flags)
{
	return ImGui::ColorEdit3(label, (float*)pColor, flags);
}

bool ImGui::ColorEdit4(const char* label, const Color* pColor, ImGuiColorEditFlags flags)
{
	return ImGui::ColorEdit4(label, (float*)pColor, flags);
}

bool ImGui::ColorEdit4(const char* label, const Vector4* pColor, ImGuiColorEditFlags flags)
{
	return ImGui::ColorEdit4(label, (float*)pColor, flags);
}

void ImGui::EditTransformHierarchy(Transform* pTransform)
{
	auto ObjectEditUI = [](GameObject* object)
		{
			bool isEdit = false;
			ImGui::Checkbox("Active", &object->Active);
			if (object->transform.Parent)
			{
				isEdit |= ImGui::DragVector3("Position", &object->transform.localPosition, 0.1f);
				isEdit |= ImGui::DragQuaternionLocal("Rotation", &object->transform.localRotation);
				isEdit |= ImGui::DragVector3("Scale", &object->transform.localScale, 0.1f);
			}
			else
			{
				isEdit |= ImGui::DragVector3("Position", &object->transform.position, 0.1f);
				isEdit |= ImGui::DragQuaternionWorld("Rotation", &object->transform.rotation);
				isEdit |= ImGui::DragVector3("Scale", &object->transform.scale, 0.1f);
			}
			if (isEdit)
				object->transform.PushUpdateList();
		};
	std::function<void(Transform* transform)> TransformBFS = [&](Transform* transform)
		{		
			static std::string nodeName;			
			nodeName = transform->gameObject.GetNameToString();
			nodeName += "##";
			nodeName += std::to_string(g_id);
			unsigned int childCount = transform->GetChildCount();
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth;
			bool isSelect = Scene::GuizmoSetting.SelectObject == &transform->gameObject;
			if (isSelect)
				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.0f, 0.7f, 1.0f));
			if (ImGui::TreeNodeEx(nodeName.c_str(), flags))
			{
				if (isSelect)
					ImGui::PopStyleColor();

				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					//Debug_wprintf(L"%s\n", transform->gameObject.Name.c_str()); 
				    Scene::GuizmoSetting.SelectObject = &transform->gameObject;

				ObjectEditUI(&transform->gameObject);
				for (size_t i = 0; i < childCount; ++i)
				{
					TransformBFS(transform->GetChild(i));
				}
				ImGui::TreePop();
			}
			else
			{
				if(isSelect)
					ImGui::PopStyleColor();

				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					//Debug_wprintf(L"%s\n", transform->gameObject.Name.c_str());
				    Scene::GuizmoSetting.SelectObject = &transform->gameObject;
			}
		};	
	TransformBFS(pTransform);
	g_id++;
}

void ImGui::EditHierarchyView()
{
	const ObjectList& objects = sceneManager.GetObjectList();
	PushID("EditHierarchyView");
	for (auto& object : objects)
	{
		if (object->transform.Parent == nullptr)
		{		
			EditTransformHierarchy(&object->transform);
		}		
	}
	PopID();
}

void ImGui::EditD3DRenderer()
{
	ImGui::PushID(g_id);
	ImGui::Text("D3DRenderer");
	ImGui::Text("FPS : %d", TimeSystem::Time.GetFrameRate());
	ImGui::Text("Draw call : %llu", d3dRenderer.GetDrawCount());
	if (ImGui::Button("Recompile Shader"))
	{
		MeshRender::ReloadShaderAll();
		d3dRenderer.CreateDeferredResource();
	}	
	if (ImGui::Button("Toggle Fullscreen"))
		d3dRenderer.ToggleFullscreenMode();
	ImGui::ColorEdit3("Clear Color", &d3dRenderer.backgroundColor);
	ImGui::ColorEdit4("Debug Draw Color", (float*)&d3dRenderer.debugDrawColor);
	ImGui::Checkbox("Lock Camera Frustum", &d3dRenderer.DebugLockCameraFrustum);
	ImGui::Checkbox("Draw Camera Frustum", &d3dRenderer.DebugDrawCameraFrustum);
	ImGui::Checkbox("Draw Light Frustum", &d3dRenderer.DebugDrawLightFrustum);
	ImGui::Checkbox("Draw Object Culling Box", &d3dRenderer.DebugDrawObjectCullingBox);
	ImGui::Checkbox("VSync", &d3dRenderer.setting.UseVSync);
	ImGui::Text("Viewport");
	{
		SIZE size = D3D11_GameApp::GetClientSize();
		float sizeWidth = (float)size.cx;
		float sizeHeight = (float)size.cy;
		D3D11_VIEWPORT& viewPort = d3dRenderer.ViewPortsVec.front();
		ImGui::SliderFloat("Width", (float*)&viewPort.Width, 100, sizeWidth);
		viewPort.Width = std::clamp(viewPort.Width, 100.f, sizeWidth);

		ImGui::SliderFloat("Height", (float*)&viewPort.Height, 100, sizeHeight);
		viewPort.Height = std::clamp(viewPort.Height, 100.f, sizeHeight);

		ImGui::SliderFloat("TopLeftX", (float*)&viewPort.TopLeftX, 0, size.cx - viewPort.Width);
		viewPort.TopLeftX = std::clamp(viewPort.TopLeftX, 0.f, size.cx - viewPort.Width);

		ImGui::SliderFloat("TopLeftY", (float*)&viewPort.TopLeftY, 0, size.cy - viewPort.Height);
		viewPort.TopLeftY = std::clamp(viewPort.TopLeftY, 0.f, size.cy - viewPort.Height);
	}
	ImGui::Text("");
	ImGui::PopID();
	g_id++;
}

void ImGui::EditTransform(GameObject* gameObject)
{
	ImGui::PushID(g_id);
	bool isEdit = false;
	if (gameObject->transform.Parent)
	{	
		isEdit |= ImGui::DragVector3("Position", &gameObject->transform.localPosition, 0.1f);
		isEdit |= ImGui::DragQuaternionLocal("Rotation", &gameObject->transform.localRotation);
		isEdit |= ImGui::DragVector3("Scale", &gameObject->transform.localScale, 0.1f);
	}
	else
	{
		isEdit |= ImGui::DragVector3("Position", &gameObject->transform.position, 0.1f);
		isEdit |= ImGui::DragQuaternionWorld("Rotation", &gameObject->transform.rotation);
		isEdit |= ImGui::DragVector3("Scale", &gameObject->transform.scale, 0.1f);
	}
	if (isEdit)
		gameObject->transform.PushUpdateList();
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

void ImGui::ShowCompressPopup(const wchar_t* path, ID3D11ShaderResourceView** ppOut_ResourceView)
{
	auto popupFunc = [=]()
		{
			// 팝업을 열기 위해 반드시 OpenPopup 호출
			ImGui::OpenPopup("Compress Texture");

			// 모달 팝업 시작
			if (ImGui::BeginPopupModal("Compress Texture"))
			{


				// 확인 버튼
				if (ImGui::Button("OK"))
				{
					textureManager.CreateSharingCompressTexture(path, ppOut_ResourceView, Utility::E_COMPRESS::BC7);
					ImGui::CloseCurrentPopup();
					sceneManager.PopImGuiPopupFunc();
				}
				ImGui::EndPopup();
			}
		};
	sceneManager.PushImGuiPopupFunc(popupFunc);
}