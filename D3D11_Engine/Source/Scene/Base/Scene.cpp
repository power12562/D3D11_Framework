#include "Scene.h"
#include <Manager/SceneManager.h>
#include <D3DCore/D3DRenderer.h>
#include <Manager/HLSLManager.h>
#include <Manager/ResourceManager.h>
#include <D3DCore/D3DSamplerState.h>
#include <typeinfo>
#include <Utility/ImguiHelper.h>
#include <Utility/Console.h>
#include <Utility/SpinLock.h>
#include <GameObject/Base/GameObject.h>
#include <Component/Camera/Camera.h>
#include <D3DCore/D3D11_GameApp.h>
#include <Math/Mathf.h>
#include <GameObject/Base/CameraObject.h>
#include <Core/DXTKInputSystem.h>
#include <algorithm>
#include <ImGuizmo/ImGuizmo.h>

Scene::Scene()
{
	constexpr unsigned int ReserveSize = 100000;
	sceneResourceList.reserve(ReserveSize);
	objectList.reserve(ReserveSize);
	dontdestroyonloadList.reserve(ReserveSize);
	d3dRenderer.reserveRenderQueue(ReserveSize);
}

Scene::~Scene()
{
	hlslManager.ClearSharingShader();
	Resource::ClearResourceManagers();
	D3DSamplerState::ClearSamplerResources();
}

void Scene::FixedUpdate()
{
	for (auto& obj : objectList)
	{
		if (obj && obj->Active)
			obj->FixedUpdate();
	}
}

void Scene::Update()
{
	for (auto& obj : objectList)
	{
		if (obj && obj->CheckActive())
		{
			obj->checkActive = obj->Active;
			if (obj->transform.GetChildCount())
				obj->UpdateChildActive(&obj->transform);
		}

		if (obj && obj->Active)
			obj->Update();
	}
}

void Scene::LateUpdate()
{
	for (auto& obj : objectList)
	{
		if (obj && obj->Active)
			obj->LateUpdate();
	}
}

void Scene::Render()
{
	d3dRenderer.BegineDraw();	
	for (auto& obj : objectList)
	{
		if (obj && obj->Active)
			obj->Render();
	}
	d3dRenderer.EndDraw();

	if (UseImGUI)
	{
		ImGUIBegineDraw();
		ImGuizmoDraw();
		ImGUIRender();
		ImGUIEndDraw();
	}
	d3dRenderer.Present();
}

void Scene::ImGUIBegineDraw()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Scene::ImGuizmoDraw()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;
	if (GuizmoSetting.UseImGuizmo)
	{
		if (Camera* mainCamera = Camera::GetMainCamera())
		{
			SIZE clientSize = D3D11_GameApp::GetClientSize();
			const Matrix& cameraVM = mainCamera->GetVM();
			const Matrix& cameraPM = mainCamera->GetPM();

			Mouse::ButtonStateTracker& mouseTracker = DXTKinputSystem.GetMouseStateTracker();
			bool isNotRightClickHELD = mouseTracker.rightButton != Mouse::ButtonStateTracker::HELD;
			if (!ImGuizmo::IsOver() && !ImGuizmo::IsUsing() && isNotRightClickHELD)
			{			
				if (mouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED)
				{
					Mouse::State state = DXTKinputSystem.GetMouse().GetState();
					Ray ray = mainCamera->ScreenPointToRay(state.x, state.y);
					float Dist = 0;
					ObjectList list = sceneManager.GetObjectList();
					std::sort(list.begin(), list.end(), [mainCamera](GameObject* a, GameObject* b)
						{
							auto fastDistance = [](const Vector3& p1, const Vector3& p2) {
								Vector3 diff = p1 - p2;
								return diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
								};
							float disA = fastDistance(mainCamera->transform.position, a->transform.position);
							float disB = fastDistance(mainCamera->transform.position, b->transform.position);
							return disA < disB;
						});

					for (auto& obj : list)
					{
						if (typeid(CameraObject) == typeid(*obj))
							continue;

						if (obj->isCulling && obj->GetOBBToWorld().Intersects(ray.position, ray.direction, Dist))
						{
							GuizmoSetting.SelectObject = obj->transform.RootParent ? &obj->transform.RootParent->gameObject : obj;
							break;
						}
					}
				}
			}

			if (GuizmoSetting.SelectObject)
			{
				ImGuizmo::BeginFrame();
				ImGuizmo::SetRect(0, 0, (float)clientSize.cx, (float)clientSize.cy);
				ImGuizmo::OPERATION operation = (ImGuizmo::OPERATION)GuizmoSetting.operation;
				ImGuizmo::MODE mode = (ImGuizmo::MODE)GuizmoSetting.mode;

				//Draw Guizmo
				{
					const float* cameraView = reinterpret_cast<const float*>(&cameraVM);
					const float* cameraProjection = reinterpret_cast<const float*>(&cameraPM);

					bool isParent = GuizmoSetting.SelectObject->transform.RootParent != nullptr;

					Matrix objMatrix = GuizmoSetting.SelectObject->transform.GetWM();
					float* pMatrix = reinterpret_cast<float*>(&objMatrix);
					ImGuizmo::Manipulate(cameraView, cameraProjection, operation, mode, pMatrix);

					Vector3 postion, scale;
					Quaternion rotation;

					objMatrix.Decompose(scale, rotation, postion);
					GuizmoSetting.SelectObject->transform.position = postion;
					GuizmoSetting.SelectObject->transform.rotation = rotation;
					GuizmoSetting.SelectObject->transform.scale = scale;
				}

				if (isNotRightClickHELD)
				{
					//Button Setting
					Keyboard::KeyboardStateTracker& keyboardTracker = DXTKinputSystem.GetKeyboardStateTracker();
					if (keyboardTracker.IsKeyPressed(GuizmoSetting.KeySetting.TRANSLATE))
					{
						GuizmoSetting.operation = ImGuizmo::OPERATION::TRANSLATE;
					}
					else if (keyboardTracker.IsKeyPressed(GuizmoSetting.KeySetting.ROTATE))
					{
						GuizmoSetting.operation = ImGuizmo::OPERATION::ROTATE;
					}
					else if (keyboardTracker.IsKeyPressed(GuizmoSetting.KeySetting.SCALE))
					{
						GuizmoSetting.operation = ImGuizmo::OPERATION::SCALE;
					}
					else if (keyboardTracker.IsKeyPressed(GuizmoSetting.KeySetting.UNIVERSAL))
					{
						GuizmoSetting.operation = ImGuizmo::OPERATION::UNIVERSAL;
					}
					else if (keyboardTracker.IsKeyPressed(GuizmoSetting.KeySetting.MODE))
					{
						GuizmoSetting.mode = (GuizmoSetting.mode != ImGuizmo::MODE::WORLD) ? ImGuizmo::MODE::WORLD : ImGuizmo::MODE::LOCAL;
					}
					if (keyboardTracker.IsKeyPressed(Keyboard::Keys::Escape))
					{
						GuizmoSetting.SelectObject = nullptr;
					}
				}
			}		
		}
	}
}

void Scene::ImGUIEndDraw()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::ResetGlobalID();
}

void Scene::SetResouceObj(const wchar_t* key, GameObject* obj)
{
	sceneResourceList[key].emplace_back(obj->GetWeakPtr().lock());
	unsigned int childCount = obj->transform.GetChildCount();
	if (childCount > 0)
	{
		for (unsigned int i = 0; i < childCount; i++)
		{
			SetResouceObj(key, &obj->transform.GetChild(i)->gameObject);
		}
	}
}

void Scene::RemoveResouceObj(const wchar_t* key)
{
	auto findIter = sceneResourceList.find(key);
	if (findIter != sceneResourceList.end())
	{
		findIter->second.clear();
	}
	else
	{
		__debugbreak();
		Debug_printf("Resource not found.\n");
	}
}

void Scene::ClearResouceObj()
{
	sceneResourceList.clear();
}

