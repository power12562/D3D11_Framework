#pragma once
#include <Scene/Base/Scene.h>

class DeferredTestScene1 : public Scene
{
public:
	DeferredTestScene1() = default;
	virtual ~DeferredTestScene1() override = default;
	virtual void Start() override;

	virtual void ImGUIRender();

	class SkyBoxObject* pSkyBox = nullptr;
	class Camera* pCamera = nullptr;
	class CameraMoveHelper* pCameraMoveHelper = nullptr;
	class SphereObject* Sphere = nullptr;
	std::vector<class MeshRender*> chrMeshs;

	bool deferredFlag = true;
};