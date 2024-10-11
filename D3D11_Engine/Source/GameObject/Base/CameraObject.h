#pragma once
#include <GameObject/Base/GameObject.h>
#include <Component/Camera/Camera.h>

class CameraObject : public GameObject
{
public:
	CameraObject();
	virtual ~CameraObject() override;

public:
	void SetMainCamera();
	
private:
	Camera* cam = nullptr;
};