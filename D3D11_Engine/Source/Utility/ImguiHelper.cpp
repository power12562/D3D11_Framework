#include "ImguiHelper.h"
#include "Math\Mathf.h"

#include <framework.h>
#include <D3DCore/D3DRenderer.h>

#include <unordered_map>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>

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

namespace CompressPopupField
{
	static int popupCount = 0;
	static std::queue<std::string>   str_queue;
	static std::queue<std::wstring>  wstr_queue;
	static std::queue<std::wstring>  savePath_Queue;
	static std::vector<std::thread>  compressThreads;
	static std::vector<ID3D11ShaderResourceView*> tempSRVvec;
	static std::unordered_set<D3DTexture2D*> textures;	//압축 후 다시 로드할 텍스쳐들
	static std::atomic_int threadsCounter;
}

bool ImGui::ShowCompressPopup(const wchar_t* path, D3DTexture2D* texture2D, int texType)
{
	using namespace CompressPopupField;
	popupCount++;
	std::filesystem::path originPath = path;
	originPath.replace_extension(L".dds");
	constexpr wchar_t textuers[] = L"Textuers";
	std::filesystem::path savePath = originPath.parent_path() / textuers / originPath.filename();
	bool isExists = std::filesystem::exists(savePath);
	str_queue.push(utfConvert::wstring_to_utf8(path));
	wstr_queue.push(path);
	savePath_Queue.push(savePath.c_str());
	ReloadTextureCompressEnd(savePath.c_str(), texture2D, texType);
	
	/*추천 포멧!!
	Albedo		BC1/BC3/BC7	 알파 채널 유무에 따라 선택. 색상 데이터의 높은 품질 유지 필요시 BC7 //생각보다 압축티 많이남..
	Normal		BC5	         2채널 사용하는 노말은 BC5. (그냥 압축 하지마세요..)
	Specular	BC1/BC7		 단순 데이터면 BC1, 고품질 필요 시 BC7.
	Emissive	BC1/BC3/BC7  불투명은 BC1, 알파 필요 시 BC3. 고품질 필요시 BC7
	Opacity		BC4/BC3		 단일 채널은 BC4, RGBA 필요 시 BC3.
	Metalness	BC4			 단일 채널 데이터로 효율적인 압축.
	Roughness	BC4			 단일 채널 데이터로 미세 디테일 유지.
	AO			BC4			 단일 채널 데이터로 충분한 품질 유지.
	*/
	constexpr const char* compressTypeStr[Utility::E_COMPRESS::MAX] =
	{
		"None",
		"BC1 ",
		"BC3 ",
		"BC4",
		"BC5",
		"BC6",
		"BC7"
	};
	constexpr const char* textureTypeStr[E_TEXTURE::PBRTextureCount] =
	{
		"Albedo",
		"Normal",
		"Specular",
		"Emissive",
		"Opacity",
		"Metalness",
		"Roughness",
		"Ambient Occulusion"
	};

	auto popupFunc = [texType, compressTypeStr, textureTypeStr, isExists]()
		{
			std::wstring& wstr_path = wstr_queue.front();
			std::string& str_path = str_queue.front();
			std::wstring& save_path = savePath_Queue.front();

			ImGui::OpenPopup("Compress Texture");
			ImGui::SetNextWindowSize(ImVec2(750, 480));
			if (ImGui::BeginPopupModal("Compress Texture", nullptr, ImGuiWindowFlags_NoResize))
			{
				static bool initialized = false;
				static bool UseAutoCompress = false;
				static int textureType = 0;
				static Utility::E_COMPRESS::TYPE compressType = Utility::E_COMPRESS::None;
				static bool showAdvancedSettings = false;
				if (!initialized)
				{
					textureType = texType;
					initialized = true;
				}

				ImGui::Text(str_path.c_str());
				ImGui::Checkbox("Use Advanced", &showAdvancedSettings);
				if (showAdvancedSettings)
				{
					ImGui::Combo("Compress Type", (int*)&compressType, compressTypeStr, Utility::E_COMPRESS::MAX);
				}
				else
				{
					ImGui::Combo("Texture Type", &textureType, textureTypeStr, E_TEXTURE::PBRTextureCount);
					{
						E_TEXTURE::TYPE type = (E_TEXTURE::TYPE)textureType;
#ifdef _DEBUG
						compressType = Utility::E_COMPRESS::None;
#else //Release
						switch (type)
						{
						case E_TEXTURE::Normal:
							compressType = Utility::E_COMPRESS::None;
							break;
						case E_TEXTURE::Albedo:
						case E_TEXTURE::Specular:
						case E_TEXTURE::Emissive:
						case E_TEXTURE::Opacity:
							compressType = Utility::E_COMPRESS::BC7;
							break;
						case E_TEXTURE::Metalness:
						case E_TEXTURE::Roughness:
						case E_TEXTURE::AmbientOcculusion:
							compressType = Utility::E_COMPRESS::BC4;
							break;
						default :
							compressType = Utility::E_COMPRESS::None;
						}
#endif
					}
				}
				ImGui::Button("Auto Compress", &UseAutoCompress);
				if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGuiKey_Enter) || UseAutoCompress)
				{								
						auto compressThreadFunc = [save_path, path = wstr_path, compType = compressType, isExists]()
							{
								static std::mutex mt;
								std::shared_ptr<DirectX::ScratchImage> image;
								mt.lock();
								Utility::CheckHRESULT(CoInitializeEx(nullptr, COINIT_MULTITHREADED)); //작업 스레드의 Com 객체 사용 활성화
								if (!textureManager.IsTextureLoaded(path.c_str()))
								{							
									ID3D11ShaderResourceView* tempSRV;
									std::filesystem::path filePath = path;
									if (isExists)
										Utility::CheckHRESULT(Utility::CreateTextureFromFile(d3dRenderer.GetDevice(), save_path.c_str(), nullptr, &tempSRV));
									else
										image = Utility::CreateCompressTexture(d3dRenderer.GetDevice(), path.c_str(), nullptr, &tempSRV, compType);
									textureManager.InsertTexture(path.c_str(), tempSRV);
									tempSRVvec.push_back(tempSRV);
									CoUninitialize();					
								}
								mt.unlock();
								if (image)
								{
									Utility::SaveTextureForDDS(save_path.c_str(), image);
								}	
								return;
							};
						compressThreads.emplace_back(compressThreadFunc); //스레드 처리
						
					str_queue.pop();
					wstr_queue.pop();
					savePath_Queue.pop();
					ImGui::CloseCurrentPopup();
					sceneManager.PopImGuiPopupFunc();
					popupCount--;
					initialized = false;
					if (popupCount == 0)
					{
						UseAutoCompress = false;

						threadsCounter = compressThreads.size();
						auto lodingWaitThreadsFunc = []()
							{
								++threadsCounter;
								for (auto& threads : compressThreads)
								{
									threads.join();
									--threadsCounter;
								}
								for (auto& texture : textures)
								{
									texture->ReloadTexture();
								}
								for (auto& tempSRV : tempSRVvec)
								{
									tempSRV->Release();
								}
								compressThreads.clear();
								compressThreads.shrink_to_fit();
								textures.clear();
								textures.rehash(0);
								tempSRVvec.clear();
								tempSRVvec.shrink_to_fit();
								--threadsCounter;
								return;
							};
						std::thread lodingWaitThread(lodingWaitThreadsFunc);
						lodingWaitThread.detach();

						sceneManager.SetLodingImguiFunc([]() 
							{
								static std::string text;
								text = std::format("Compress : {}", threadsCounter.load());
								ImGuiIO& io = ImGui::GetIO();
								ImDrawList* draw_list = ImGui::GetForegroundDrawList();
								ImVec2 screen_size = io.DisplaySize;
								draw_list->AddRectFilled(ImVec2(0, 0), screen_size, IM_COL32(255, 255, 255, 255));
								ImVec2 text_size = ImGui::CalcTextSize(text.c_str());
								ImVec2 text_pos = ImVec2(
									(screen_size.x - text_size.x) * 0.5f, 
									(screen_size.y - text_size.y) * 0.5f
								);
								draw_list->AddText(
									NULL, 50.0f,
									text_pos,
									IM_COL32(0, 0, 0, 255),
									text.c_str()
								);

								if (threadsCounter == 0)
								{
									sceneManager.EndLodingImguiFunc();
								}
							});
					}
				}
				ImGui::EndPopup();
			}
		};

	bool ActiveImgui = sceneManager.IsImGuiActive();
	if (ActiveImgui)
	{
		sceneManager.PushImGuiPopupFunc(popupFunc);
		return true;
	}	
	else
		return false;
}

bool ImGui::ReloadTextureCompressEnd(const wchar_t* path, D3DTexture2D* texture2D, int texType)
{
	using namespace CompressPopupField;
	if (popupCount > 0)
	{
		texture2D->GetPath(texType) = path;
		textures.insert(texture2D);
		return true;
	}
	return false;
}
