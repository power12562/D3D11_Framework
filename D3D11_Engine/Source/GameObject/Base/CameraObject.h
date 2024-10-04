#pragma once
#include <GameObject/Base/GameObject.h>
#include <Component/Camera.h>

class CameraObject : public GameObject
{
public:
	CameraObject(const wchar_t* name);
	virtual ~CameraObject() override;

};